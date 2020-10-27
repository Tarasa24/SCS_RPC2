#pragma once

#include <string>
#include <chrono>
#include <vector>

#include "cities_ets2.h"
#include "cities_ats.h"
#include "custom_types.h"

scs_log_t game_log = NULL;
void log_ingame(const std::string msg) {
	game_log(SCS_LOG_TYPE_message, ("SCS_RPC | " + msg).c_str());
}

long seconds_since_epoch() {
	const auto p1 = std::chrono::system_clock::now();
	return static_cast<long>(std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count());
}

std::string to_lower(std::string s) {
	for (char& c : s) {
		if (isspace(c)) c = *"_";
		else c = tolower(c);
	}
	return s;
}

void set_activity(std::string details, std::string state, std::string smallImage, std::string smallText, telemetry_state* telemetry) {
	discord::Activity activity{};
	activity.SetDetails(details.c_str());
	activity.SetState(state.c_str());
	activity.GetTimestamps().SetStart(telemetry->start_timestamp);
	activity.GetAssets().SetSmallImage(smallImage.c_str());
	activity.GetAssets().SetSmallText(smallText.c_str());
	activity.GetAssets().SetLargeImage("cover");
	activity.GetAssets().SetLargeText(u8"Made with \u2764\uFE0F by Tarasa24");
	telemetry->discordCore->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
}

tree2d* construct_tree(telemetry_state* telemetry) {
	city* cities = NULL;
	size_t length;

	switch (telemetry->game)
	{
	case (ETS2):
		cities = cities_ets2;
		length = sizeof(cities_ets2) / sizeof(cities_ets2[0]);
		break;
	case (ATS):
		cities = cities_ats;
		length = sizeof(cities_ats) / sizeof(cities_ats[0]);
		break;
	default:
		return NULL;
		break;
	}

	std::vector<point2d> points;

	for (size_t i = 0; i < length; i++)
	{
		city* city = &cities[i];
		point2d p{ city->x, city->z };
		p.name = city->name;
		p.country = city->country;

		points.push_back(p);
	}

	return new tree2d(std::begin(points), std::end(points));
}