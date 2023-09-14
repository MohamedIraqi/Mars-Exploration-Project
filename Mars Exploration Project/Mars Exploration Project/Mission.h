#pragma once

#include "Definitions.h"
#include "Rover.h"
#include <cmath>
#include <cstdlib>

class Mission
{
private:
	const MissionTypes missionType;	//TYP is the mission type (M: mountainous, P: polar, E: emergency).
	int formulationDay;
	const int ID;
	const int targetLocation;		//TLOC is the mission’s target location(in kilometers from the base station).
	const int missionDuration;		//MDUR is the number of days needed to fulfill the mission requirements at target location.
	const int significance;			//SIG is the mission’s significance.
	int waitingTime;
	int totalMissionTime;
	int completionDay;
	int failureDay;
	Rover* assignedRover;

public:
	Mission(MissionTypes r_MissionType, int r_FormulationDay, int r_ID, int r_TargetLocation, int r_MissionDuration, int r_Significance);
	MissionTypes getMissionType() const;
	void setFormulationDay(int r_FormulationDay);
	int getFormulationDay() const;
	int getID() const;
	int getTargetLocation() const;
	int getMissionDuration() const;
	int getSignificance() const;
	void setStatistics(int r_CurrentDay);
	int getWaitingTime() const;
	int getTotalMissionTime() const;
	void setCompletionDay(int r_CompletionDay);
	int getCompletionDay() const;
	int getFailureDay() const;
	bool didMissionFail() const;
	void setAssignedRover(Rover*);
	Rover* getAssignedRover() const;
};

