#pragma once

#include <string>
#include <memory>

#include "lib/kd_tree.h"
#include "lib/sdk/discord sdk/cpp/discord.h"

typedef point<double, 2> point2d;
typedef kdtree<double, 2> tree2d;

 struct city {
	std::string name;
	std::string country;
	double x;
	double y;
	double z;
};

 enum Game { ETS2, ATS };

 struct position
{
	bool avalible = false;
	double x;
	double y;
	double z;
	std::string closestCity;
	std::string country;
};

 struct cargo
{
	std::string name;
	float weight;

	bool loaded = false;
	bool trailer_connected = true;
};

 struct truck
{
	std::string brand;
	std::string name;
};

 struct telemetry_state {
	std::unique_ptr<discord::Core> discordCore;

	long start_timestamp;
	std::unique_ptr<tree2d> cities{};

	bool paused = true;
	int game;

	position position;
	float speed;
	float distance;

	cargo cargo;
	truck truck;
};
