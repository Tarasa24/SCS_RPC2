#pragma once

#include <string>
#include <sstream>

#include "custom_types.h"
#include "helper_functions.h"

SCSAPI_VOID handle_frame_start(const scs_event_t UNUSED(event), const void* const UNUSED(event_info), const scs_context_t context) {
	telemetry_state* const telemetry = static_cast<telemetry_state*>(context);

	std::stringstream state;
	state << "Close to " << telemetry->position.closestCity << " (" << telemetry->position.country << ")";

	std::stringstream details;
	if (telemetry->game == ETS2) details << telemetry->cargo.name << " (" << (int)(telemetry->cargo.weight / 1000.0f) << " t) | " << (int)(telemetry->distance / 1000.0f) << " km";
	else if (telemetry->game == ATS) details << telemetry->cargo.name << " (" << (int)(telemetry->cargo.weight / 1000.0f) << " t) | " << (int)(telemetry->distance / 1609.0f) << " mi";
	bool on_job = telemetry->cargo.loaded && telemetry->cargo.trailer_connected;

	std::stringstream small_image;
	if (!telemetry->truck.brand.empty()) small_image << to_lower(telemetry->truck.brand);

	std::stringstream small_text;
	if (telemetry->game == ETS2) small_text << telemetry->truck.brand << " " << telemetry->truck.name <<" (" << (int)(telemetry->speed * 3.6) << " km/h)";
	else if (telemetry->game == ATS) small_text << telemetry->truck.brand << " " << telemetry->truck.name << " (" << (int)(telemetry->speed * 2.237) << " mph)";

	set_activity(!telemetry->position.avalible || telemetry->paused ? "Paused" : on_job ? details.str() : u8"🚚 Free roaming 💨",
				!telemetry->position.avalible || telemetry->paused ? "" : state.str(),
				small_image.str(), small_text.str(), telemetry);

	telemetry->discordCore->RunCallbacks();
}

SCSAPI_VOID handle_pause_start(const scs_event_t event, const void* const UNUSED(event_info), const scs_context_t context)
{
	telemetry_state* const telemetry = static_cast<telemetry_state*>(context);

	telemetry->paused = (event == SCS_TELEMETRY_EVENT_paused);
}

SCSAPI_VOID handle_position(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context) {
	telemetry_state* const telemetry = static_cast<telemetry_state*>(context);

	if (value == NULL) {
		telemetry->position.avalible = false;
		return;
	}
	telemetry->position.avalible = true;
	telemetry->position.x = value->value_dvector.x;
	telemetry->position.y = value->value_dvector.y;
	telemetry->position.z = value->value_dvector.z;

	point2d p{ (double)telemetry->position.x, (double)telemetry->position.z };
	point2d closest_city = telemetry->cities->nearest(p);

	telemetry->position.closestCity = static_cast<std::string>(closest_city.name);
	telemetry->position.country = static_cast<std::string>(closest_city.country);
}

SCSAPI_VOID handle_speed(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context) {
	if (value == NULL) return;
	telemetry_state* const telemetry = static_cast<telemetry_state*>(context);

	telemetry->speed = value->value_float.value;
}

SCSAPI_VOID handle_distance(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context) {
	if (value == NULL) return;
	telemetry_state* const telemetry = static_cast<telemetry_state*>(context);

	telemetry->distance = value->value_float.value;
}

SCSAPI_VOID handle_trailer_connect(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context) {
	if (value == NULL) return;
	telemetry_state* const telemetry = static_cast<telemetry_state*>(context);

	if (value) {
		telemetry->cargo.trailer_connected = value->value_bool.value;
	}
	else {
		telemetry->cargo.trailer_connected = false;
	}
}

SCSAPI_VOID handle_configuration(const scs_event_t event, const void* const event_info, const scs_context_t context)
{
	if (event_info == NULL) return;
	telemetry_state* const telemetry = static_cast<telemetry_state*>(context);
	const struct scs_telemetry_configuration_t* const info = static_cast<const scs_telemetry_configuration_t*>(event_info);

	std::string id = info->id;

	if (id.compare(SCS_TELEMETRY_CONFIG_job) == 0) {
		for (const scs_named_value_t* current = info->attributes; current->name; ++current) {
			const std::string name = current->name;
			if (name.compare(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo) == 0) {
				telemetry->cargo.name = static_cast<std::string>(current->value.value_string.value);
			}
			else if (name.compare(SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass) == 0) {
				telemetry->cargo.weight = current->value.value_float.value;
			}
			else if (name.compare(SCS_TELEMETRY_CONFIG_ATTRIBUTE_is_cargo_loaded) == 0) {
				telemetry->cargo.loaded = current->value.value_bool.value;
			}
		}
	}
	else if (id.compare(SCS_TELEMETRY_CONFIG_truck) == 0) {
		for (const scs_named_value_t* current = info->attributes; current->name; ++current) {
			const std::string name = current->name;
			if (name.compare(SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand) == 0) {
				telemetry->truck.brand = static_cast<std::string>(current->value.value_string.value);
			}
			else if (name.compare(SCS_TELEMETRY_CONFIG_ATTRIBUTE_name) == 0) {
				telemetry->truck.name = static_cast<std::string>(current->value.value_string.value);
			}
		}
	}
}