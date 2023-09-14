#include "Mission.h"

Mission::Mission(MissionTypes r_MissionType, int r_FormulationDay, int r_ID, int r_TargetLocation, int r_MissionDuration, int r_Significance)
	: missionType(r_MissionType), formulationDay(r_FormulationDay), ID(r_ID), targetLocation(r_TargetLocation), missionDuration(r_MissionDuration), significance(r_Significance)
{
	waitingTime = 0;
	totalMissionTime = 0;
	completionDay = 0;
	failureDay = 0;
	assignedRover = nullptr;
}

MissionTypes Mission::getMissionType() const
{
	return missionType;
}

void Mission::setFormulationDay(int r_FormulationDay)
{
	formulationDay = r_FormulationDay;
}

int Mission::getFormulationDay() const
{
	return formulationDay;
}

int Mission::getID() const
{
	return ID;
}

int Mission::getTargetLocation() const
{
	return targetLocation;
}

int Mission::getMissionDuration() const
{
	return missionDuration;
}

int Mission::getSignificance() const
{
	return significance;
}

void Mission::setStatistics(int r_CurrentDay)
{
	if (assignedRover)
	{
		waitingTime = r_CurrentDay - formulationDay;

		totalMissionTime = (int)ceil((2 * targetLocation) / (((float)assignedRover->getSpeed() / pow(2, assignedRover->getNumberOfDamage())) * HOURSPERDAY_MARS)) + missionDuration;
		completionDay = formulationDay + waitingTime + totalMissionTime;
		if (didMissionFail())
			failureDay = formulationDay + waitingTime + (rand() % totalMissionTime);
		else
			failureDay = completionDay + 1;
	}
	else
	{
		waitingTime = 0;
		totalMissionTime = 0;
		completionDay = 0;
		failureDay = 0;
	}
}

int Mission::getWaitingTime() const
{
	return waitingTime;
}

int Mission::getTotalMissionTime() const
{
	return totalMissionTime;
}

void Mission::setCompletionDay(int r_CompletionDay)
{
	completionDay = r_CompletionDay;
}

int Mission::getCompletionDay() const
{
	return completionDay;
}

int Mission::getFailureDay() const
{
	return failureDay;
}

bool Mission::didMissionFail() const
{
	int failPercentage;
	failPercentage = 1 + (rand() % 100);
	if (failPercentage <= PERCENTAGE_MISSIONFAIL)
		return true;
	else
		return false;
}

void Mission::setAssignedRover(Rover* r_AssignedRover)
{
	assignedRover = r_AssignedRover;
}

Rover* Mission::getAssignedRover() const
{
	return assignedRover;
}
