#pragma once

#define HOURSPERDAY_EARTH 24
#define HOURSPERDAY_MARS 25

#define PERCENTAGE_MISSIONFAIL 5

enum class ModesOfOperation
{
	Interactive_Mode,
	Step_By_Step_Mode,
	Silent_Mode
};

enum class EventTypes //There are 3 types of missions: Formulation, Promotion and cancellation events.
{
	Formulation_Event,
	Promotion_Event,
	cancellation_Event
};

enum class MissionTypes //There are 3 types of missions: Emergency, Mountainous and Polar missions.
{
	Emergency_Mission,
	Mountainous_Mission,
	Polar_Mission
};

enum class RoverTypes //There are 3 types of rovers: Emergency, Mountainous, and Polar rovers.
{
	Emergency_Rover,
	Mountainous_Rover,
	Polar_Rover
};