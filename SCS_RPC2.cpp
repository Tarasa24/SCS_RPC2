// Windows stuff.

#ifdef _WIN32
#  define WINVER 0x0500
#  define _WIN32_WINNT 0x0500
#  include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>

// SDKs

#include "lib/sdk/telemetry sdk/scssdk_telemetry.h"
#include "lib/sdk/telemetry sdk/eurotrucks2/scssdk_eut2.h"
#include "lib/sdk/telemetry sdk/eurotrucks2/scssdk_telemetry_eut2.h"
#include "lib/sdk/telemetry sdk/amtrucks/scssdk_ats.h"
#include "lib/sdk/telemetry sdk/amtrucks/scssdk_telemetry_ats.h"
#define UNUSED(x)

#include "lib/sdk/discord sdk/cpp/discord.h"

 // Libs
#include "lib/kd_tree.h"

 // Custom
#include "helper_functions.h"
#include "custom_types.h"
#include "handlers.h"

telemetry_state telemetry{};

SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params)
{
	memset(&telemetry, 0, sizeof(telemetry));

	const scs_telemetry_init_params_v101_t* const version_params = static_cast<const scs_telemetry_init_params_v101_t*>(params);
	game_log = version_params->common.log;

	log_ingame(version_params->common.game_name);

	discord::Core* core{};
	std::string game = version_params->common.game_id;
	discord::Result result{};
	if (game == SCS_GAME_ID_EUT2) {
		telemetry.game = ETS2;
		result = discord::Core::Create(529016610137309184, DiscordCreateFlags_NoRequireDiscord, &core);
	}
	else if (game == SCS_GAME_ID_ATS) {
		telemetry.game = ATS;
		result = discord::Core::Create(529069002874421249, DiscordCreateFlags_NoRequireDiscord, &core);
	}
	if (result == discord::Result(0)) telemetry.discordCore.reset(core);
	else return SCS_RESULT_generic_error;

	telemetry.cities = construct_tree(&telemetry);

	bool registered = (
		version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, handle_frame_start, &telemetry) == SCS_RESULT_ok &&
		version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, handle_pause_start, &telemetry) == SCS_RESULT_ok &&
		version_params->register_for_event(SCS_TELEMETRY_EVENT_started, handle_pause_start, &telemetry) == SCS_RESULT_ok &&
		version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_world_placement, SCS_U32_NIL, SCS_VALUE_TYPE_dvector, SCS_TELEMETRY_CHANNEL_FLAG_no_value, handle_position, &telemetry) == SCS_RESULT_ok &&
		version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_speed, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_no_value, handle_speed, &telemetry) == SCS_RESULT_ok &&
		version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_distance, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_no_value, handle_distance, &telemetry) == SCS_RESULT_ok &&
		version_params->register_for_channel(SCS_TELEMETRY_TRAILER_CHANNEL_connected, SCS_U32_NIL, SCS_VALUE_TYPE_bool, SCS_TELEMETRY_CHANNEL_FLAG_no_value, handle_trailer_connect, &telemetry) == SCS_RESULT_ok &&
		version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, handle_configuration, &telemetry) == SCS_RESULT_ok
	);

	if (!registered)
	{
		log_ingame("Unable to register callbacks");
		return SCS_RESULT_generic_error;
	}

	telemetry.start_timestamp = seconds_since_epoch();

	return SCS_RESULT_ok;
}

SCSAPI_VOID scs_telemetry_shutdown(void) {}

// Cleanup

#ifdef _WIN32
BOOL APIENTRY DllMain(
	HMODULE module,
	DWORD  reason_for_call,
	LPVOID reseved
)
{
	return TRUE;
}
#endif

#ifdef __linux__
void __attribute__((destructor)) unload(void)
{}
#endif
