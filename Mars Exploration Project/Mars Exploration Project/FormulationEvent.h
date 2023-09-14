#pragma once

#include "Event.h"
#include "MarsStation.h"

class FormulationEvent :public Event
{
private:
	const MissionTypes missionType;	//TYP is the mission type (M: mountainous, P: polar, E: emergency).
	const int targetLocation;		//TLOC is the mission’s target location(in kilometers from the base station).
	const int missionDuration;		//MDUR is the number of days needed to fulfill the mission requirements at target location.
	const int significance;			//SIG is the mission’s significance.
public:
	FormulationEvent(int r_EventDay, int r_ID, MissionTypes r_MissionType, int r_TargetLocation, int r_MissionDuration, int r_Significance);

	void execute(MarsStation* pMS);
};