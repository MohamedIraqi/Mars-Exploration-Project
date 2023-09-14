#include "FormulationEvent.h"

FormulationEvent::FormulationEvent(int r_EventDay, int r_ID, MissionTypes r_MissionType, int r_TargetLocation, int r_MissionDuration, int r_Significance)
	: Event(EventTypes::Formulation_Event, r_EventDay, r_ID), missionType(r_MissionType), targetLocation(r_TargetLocation), missionDuration(r_MissionDuration), significance(r_Significance)
{	}

void FormulationEvent::execute(MarsStation* pMS)
{
	pMS->formulateMission(missionType,eventDay,ID,targetLocation,missionDuration,significance);
}