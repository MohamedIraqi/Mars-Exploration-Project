#include "Event.h"

Event::Event(EventTypes r_EventType, int r_EventDay, int r_ID)
	: eventType(r_EventType), eventDay(r_EventDay), ID(r_ID)
{	}

EventTypes Event::getEventType() const
{
	return eventType;
}

int Event::getEventDay() const
{
	return eventDay;
}