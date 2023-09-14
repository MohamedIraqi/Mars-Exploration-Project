#pragma once

#include "Definitions.h"
#include "UI.h"
#include "LinkedQueue.h"
#include "LinkedPriorityQueue.h"
#include "FormulationEvent.h"
#include "Mission.h"
#include "Rover.h"
#include <fstream>
#include <chrono>
#include <thread>
using namespace std::chrono;
using namespace std::this_thread;

class MarsStation
{
private:
	UI* const pUI;

	int currentDay;

	int rovers_Num_Polar;
	int rovers_Num_Emergency;

	int missions_Num_Polar;
	int missions_Num_Emergency;

	float average_Waiting_Time;
	float average_Execution_Time;

	LinkedQueue<Event*> events;

	LinkedPriorityQueue<Mission*> missions_Waiting_Emergency;
	LinkedQueue<Mission*> missions_Waiting_Polar;

	LinkedPriorityQueue<Rover*> rovers_Waiting_Emergency;
	LinkedPriorityQueue<Rover*> rovers_Waiting_Polar;

	LinkedPriorityQueue<Mission*> missions_InExecution;

	LinkedQueue<Mission*> missions_Completed;

	LinkedPriorityQueue<Rover*> rovers_Checkup;

public:
	MarsStation(UI* const r_pUI);
	void execute();
	void readInputFile();
	void executeDay();
	void PrintOutput() const;
	void writeOutputFile();
	void formulateMission(MissionTypes r_Mission_Type, int r_Mission_Day, int r_Mission_ID, int r_Target_Location, int r_Mission_Duration, int r_Significance);
	~MarsStation();

	//excute day help functions
	void executeEvents();
	void dismissCheckupRovers();
	void dismissCompletedMissions();
	void assignRovers();
};