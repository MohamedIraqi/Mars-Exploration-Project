#pragma once

#include "Definitions.h"

class MarsStation;

class Event
{
protected:
	const EventTypes eventType;
	const int eventDay;
	const int ID;
public:
	Event(EventTypes r_EventType, int r_EventDay, int r_ID);
	virtual void execute(MarsStation* pMS) = 0;
	EventTypes getEventType() const;
	int getEventDay() const;
};