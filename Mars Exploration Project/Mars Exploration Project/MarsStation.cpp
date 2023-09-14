#include "MarsStation.h"

MarsStation::MarsStation(UI* r_pUI)
	:pUI(r_pUI)
{
	currentDay = 1;

	rovers_Num_Polar = 0;
	rovers_Num_Emergency = 0;
	missions_Num_Polar = 0;
	missions_Num_Emergency = 0;

	average_Waiting_Time = 0;
	average_Execution_Time = 0;
}

void MarsStation::execute()
{
	readInputFile();

	pUI->printString("select the program mode:");
	pUI->setModeOfOperation();
	switch (pUI->getModeOfOperation())
	{
	case ModesOfOperation::Interactive_Mode:

		cin.ignore();

		while (!events.isEmpty() || !missions_Waiting_Emergency.isEmpty() || !missions_Waiting_Polar.isEmpty() || !missions_InExecution.isEmpty() || !rovers_Checkup.isEmpty())
		{
			executeDay();
			PrintOutput();

			char tempinput = 0;
			while (tempinput != '\r' && tempinput != '\n')
			{
				cin.get(tempinput);
			}

			++currentDay;
		}

		break;

	case ModesOfOperation::Step_By_Step_Mode:

		while (!events.isEmpty() || !missions_Waiting_Emergency.isEmpty() || !missions_Waiting_Polar.isEmpty() || !missions_InExecution.isEmpty() || !rovers_Checkup.isEmpty())
		{
			executeDay();
			PrintOutput();

			pUI->printString("");
			sleep_for(seconds(1));

			++currentDay;
		}

		break;

	case ModesOfOperation::Silent_Mode:

		while (!events.isEmpty() || !missions_Waiting_Emergency.isEmpty() || !missions_Waiting_Polar.isEmpty() || !missions_InExecution.isEmpty() || !rovers_Checkup.isEmpty())
		{
			executeDay();

			++currentDay;
		}

		pUI->printString("Silent Mode");
		pUI->printString("Simulation Starts...");
		pUI->printString("Simulation ends, Output file created");
		break;

	default:
		break;
	}

	writeOutputFile();
}

void MarsStation::readInputFile()
{
	pUI->printString("What is the name (no extension) of the file you would like to read from?");

	ifstream InputFile(pUI->getUserInput() + ".txt");

	while (!InputFile.is_open())
	{
		pUI->printString("Error! File not found");

		pUI->printString("What is the name (no extension) of the file you would like to read from?");
		InputFile.open(pUI->getUserInput() + ".txt");
	}

	//Rovers
	{
		rovers_Num_Polar = 0;
		rovers_Num_Emergency = 0;
		InputFile >> rovers_Num_Polar >> rovers_Num_Emergency;

		int* speeds = new int[rovers_Num_Polar + rovers_Num_Emergency];
		int ID = 1;

		for (int i = 0; i < rovers_Num_Polar + rovers_Num_Emergency; ++i)
			InputFile >> speeds[i];

		int numMissionsBeforeCheckup;
		InputFile >> numMissionsBeforeCheckup;

		int numPolarDaysInCheckup;
		int numEmergencyDaysInCheckup;
		InputFile >> numPolarDaysInCheckup >> numEmergencyDaysInCheckup;

		for (int polarRover = 0; polarRover < rovers_Num_Polar; ++polarRover)
		{
			Rover* newRover = new Rover(RoverTypes::Polar_Rover, ID, numMissionsBeforeCheckup, numPolarDaysInCheckup, speeds[ID - 1]);
			rovers_Waiting_Polar.enqueue(newRover, newRover->getSpeed());
			++ID;
		}

		for (int emergencyRover = 0; emergencyRover < rovers_Num_Emergency; ++emergencyRover)
		{
			Rover* newRover = new Rover(RoverTypes::Emergency_Rover, ID, numMissionsBeforeCheckup, numEmergencyDaysInCheckup, speeds[ID - 1]);
			rovers_Waiting_Emergency.enqueue(newRover, newRover->getSpeed());
			++ID;
		}

		delete[] speeds;
	}

	//Events
	{
		int numEvents;
		InputFile >> numEvents;

		for (int i = 0; i < numEvents; ++i)
		{
			Event* newEvent = nullptr;

			char event_Type;
			InputFile >> event_Type;
			switch (event_Type)
			{
			case 'F':
				char mission_Type;
				InputFile >> mission_Type;

				int event_Day;
				int event_ID;
				int mission_Target_Location;
				int mission_Duration;
				int mission_Significance;
				InputFile >> event_Day >> event_ID >> mission_Target_Location >> mission_Duration >> mission_Significance;

				switch (mission_Type)
				{
				case 'E':
					newEvent = new FormulationEvent(event_Day, event_ID, MissionTypes::Emergency_Mission, mission_Target_Location, mission_Duration, mission_Significance);
					break;
				case 'P':
					newEvent = new FormulationEvent(event_Day, event_ID, MissionTypes::Polar_Mission, mission_Target_Location, mission_Duration, mission_Significance);
					break;
				default:
					break;
				}
				events.enqueue(newEvent);
				break;
			default:
				break;
			}
		}
	}

	InputFile.close();
}

void MarsStation::executeDay()
{
	executeEvents();
	dismissCheckupRovers();
	dismissCompletedMissions();
	assignRovers();
};

void MarsStation::PrintOutput() const
{
	pUI->printString("----------------------------------------------------------------------------------------------------");
	pUI->printString("Current Day: " + to_string(currentDay));
	pUI->printString(to_string(missions_Waiting_Emergency.getCount() + missions_Waiting_Polar.getCount()) + " Waiting Missions: " + missions_Waiting_Emergency.toString() + " " + missions_Waiting_Polar.toString());
	pUI->printString("----------------------------------------------------------------------------------------------------");
	pUI->printString(to_string(missions_InExecution.getCount()) + " In-Execution Missions/Rovers: " + missions_InExecution.toString());
	pUI->printString("----------------------------------------------------------------------------------------------------");
	pUI->printString(to_string(rovers_Waiting_Emergency.getCount() + rovers_Waiting_Polar.getCount()) + " Available Rovers: " + rovers_Waiting_Emergency.toString() + " " + rovers_Waiting_Polar.toString());
	pUI->printString("----------------------------------------------------------------------------------------------------");
	pUI->printString(to_string(rovers_Checkup.getCount()) + " In-Checkup Rovers: " + rovers_Checkup.toString());
	pUI->printString("----------------------------------------------------------------------------------------------------");
	pUI->printString(to_string(missions_Completed.getCount()) + " Completed Missions: " + missions_Completed.toString());
	pUI->printString("----------------------------------------------------------------------------------------------------");
}

void MarsStation::writeOutputFile()
{
	pUI->printString("What is the name (no extension) of the file you would like to write to?");
	ofstream OutputFile(pUI->getUserInput() + ".txt");

	OutputFile << "CD" << '\t' << "ID" << '\t' << "FD" << '\t' << "WD" << '\t' << "ED" << endl;

	int total_Waiting_Time = 0;
	int total_Execution_Time = 0;

	Mission* Mis;

	while (missions_Completed.peek(Mis))
	{
		missions_Completed.dequeue(Mis);

		if (Mis->getFailureDay() < Mis->getCompletionDay())
			OutputFile << to_string(Mis->getFailureDay()) << "|X" << '\t' << to_string(Mis->getID()) << '\t' << to_string(Mis->getFormulationDay()) << '\t'
			<< to_string(Mis->getWaitingTime()) << '\t' << to_string(Mis->getTotalMissionTime() - (Mis->getCompletionDay() - Mis->getFailureDay())) << endl;
		else
		{
			OutputFile << to_string(Mis->getCompletionDay()) << '\t' << to_string(Mis->getID()) << '\t' << to_string(Mis->getFormulationDay()) << '\t'
			<< to_string(Mis->getWaitingTime()) << '\t' << to_string(Mis->getTotalMissionTime()) << endl;

			total_Waiting_Time += Mis->getWaitingTime();
			total_Execution_Time += Mis->getTotalMissionTime();
		}

		delete Mis;
	}

	OutputFile << endl;

	average_Waiting_Time = (float)total_Waiting_Time / (missions_Num_Polar + missions_Num_Emergency);
	average_Execution_Time = (float)total_Execution_Time / (missions_Num_Polar + missions_Num_Emergency);

	OutputFile << "Missions: " << to_string(missions_Num_Polar + missions_Num_Emergency) << " " << "[" << "P: " << to_string(missions_Num_Polar) << ", " << "E: " << to_string(missions_Num_Emergency) << "]" << endl;
	OutputFile << "Rovers: " << to_string(rovers_Num_Polar + rovers_Num_Emergency) << " " << "[" << "P: " << to_string(rovers_Num_Polar) << ", " << "E: " << to_string(rovers_Num_Emergency) << "]" << endl;
	OutputFile << "Avg Wait = " << to_string(average_Waiting_Time) << ", " << "Avg Exec = " << to_string(average_Execution_Time) << endl;

	OutputFile.close();
}

void MarsStation::formulateMission(MissionTypes r_Mission_Type, int r_Mission_Day, int r_Mission_ID, int r_Target_Location, int r_Mission_Duration, int r_Significance)
{
	Mission* newMission = new Mission(r_Mission_Type, r_Mission_Day, r_Mission_ID, r_Target_Location, r_Mission_Duration, r_Significance);
	switch (r_Mission_Type)
	{
	case MissionTypes::Emergency_Mission:
		missions_Waiting_Emergency.enqueue(newMission, r_Significance - ((int)ceil((2 * r_Target_Location) / ((float)1 * HOURSPERDAY_MARS)) + r_Mission_Duration));
		++missions_Num_Emergency;
		break;
	case MissionTypes::Polar_Mission:
		missions_Waiting_Polar.enqueue(newMission);
		++missions_Num_Polar;
		break;
	default:
		break;
	}
}

MarsStation::~MarsStation()
{
	Rover* Rov;

	while (rovers_Waiting_Emergency.peek(Rov))
	{
		rovers_Waiting_Emergency.dequeue(Rov);
		delete Rov;
	}
	while (rovers_Waiting_Polar.peek(Rov))
	{
		rovers_Waiting_Polar.dequeue(Rov);
		delete Rov;
	}
}

//excute day help functions
#pragma region ExecuteDay
void MarsStation::executeEvents()
{
	Event* Ev;
	while (events.peek(Ev))
	{
		if (Ev->getEventDay() <= currentDay)
		{
			events.dequeue(Ev);
			Ev->execute(this);
			delete Ev;
		}
		else
			break;
	}
}

void MarsStation::dismissCheckupRovers()
{
	Rover* Rov;
	while (rovers_Checkup.peek(Rov))
	{
		if (Rov->getCurrentExitDayInCheckup() <= currentDay)
		{
			rovers_Checkup.dequeue(Rov);
			switch (Rov->getRoverType())
			{
			case RoverTypes::Emergency_Rover:
				Rov->setCurrentExitDayInCheckup(-1);
				rovers_Waiting_Emergency.enqueue(Rov, Rov->getSpeed());
				break;
			case RoverTypes::Polar_Rover:
				Rov->setCurrentExitDayInCheckup(-1);
				rovers_Waiting_Polar.enqueue(Rov, Rov->getSpeed());
			default:
				break;
			}
		}
		else
			break;
	}
}

void MarsStation::dismissCompletedMissions()
{
	Mission* Mis;
	Rover* Rov;

	while (missions_InExecution.peek(Mis))
	{
		if (Mis->getFailureDay() <= currentDay)
		{
			missions_InExecution.dequeue(Mis);
			Rov = Mis->getAssignedRover();
			Mis->setAssignedRover(nullptr);

			if (Rov)
			{
				Rov->setCurrentExitDayInCheckup(currentDay + Rov->getDaysInCheckup());
				Rov->setCurrentMissionsBeforeCheckup(0);
				rovers_Checkup.enqueue(Rov, -Rov->getCurrentExitDayInCheckup());
			}

			switch (Mis->getMissionType())
			{
			case MissionTypes::Emergency_Mission:
				formulateMission(MissionTypes::Emergency_Mission, currentDay, Mis->getID(), Mis->getTargetLocation(), Mis->getMissionDuration(), Mis->getSignificance());
				--missions_Num_Emergency;
				break;
			case MissionTypes::Polar_Mission:
				formulateMission(MissionTypes::Polar_Mission, currentDay, Mis->getID(), Mis->getTargetLocation(), Mis->getMissionDuration(), Mis->getSignificance());
				--missions_Num_Polar;
				break;
			default:
				break;
			}

			missions_Completed.enqueue(Mis);
		}
		else if (Mis->getCompletionDay() <= currentDay)
		{
			missions_InExecution.dequeue(Mis);
			Rov = Mis->getAssignedRover();
			Mis->setAssignedRover(nullptr);

			if (Rov->getMissionsBeforeCheckup() <= Rov->getCurrentMissionsBeforeCheckup())
			{
				Rov->setCurrentExitDayInCheckup(currentDay + Rov->getDaysInCheckup());
				Rov->setCurrentMissionsBeforeCheckup(0);
				rovers_Checkup.enqueue(Rov, -Rov->getCurrentExitDayInCheckup());
			}
			else if (Rov->getCurrentExitDayInCheckup() != -1)
			{
				switch (Rov->getRoverType())
				{
				case RoverTypes::Emergency_Rover:
					rovers_Waiting_Emergency.enqueue(Rov, -1);
					break;
				case RoverTypes::Polar_Rover:
					rovers_Waiting_Polar.enqueue(Rov, -1);
					break;
				default:
					break;
				}
			}
			else
			{
				switch (Rov->getRoverType())
				{
				case RoverTypes::Emergency_Rover:
					rovers_Waiting_Emergency.enqueue(Rov, Rov->getSpeed());
					break;
				case RoverTypes::Polar_Rover:
					rovers_Waiting_Polar.enqueue(Rov, Rov->getSpeed());
					break;
				default:
					break;
				}
			}

			missions_Completed.enqueue(Mis);
		}
		else
			break;
	}
}

void MarsStation::assignRovers()
{
	Mission* Mis;
	Rover* Rov;

	while (missions_Waiting_Emergency.peek(Mis))
	{
		if (rovers_Waiting_Emergency.peek(Rov))
		{
			missions_Waiting_Emergency.dequeue(Mis);
			rovers_Waiting_Emergency.dequeue(Rov);
			Mis->setAssignedRover(Rov);
			Mis->setStatistics(currentDay);
			Rov->setCurrentMissionsBeforeCheckup(Rov->getCurrentMissionsBeforeCheckup() + 1);
			if (Rov->getCurrentExitDayInCheckup() != -1 && !(Rov->getCurrentExitDayInCheckup() <= currentDay))
			{
				Rov->setNumberOfDamage(Rov->getNumberOfDamage() + 1);
				Rov->setCurrentExitDayInCheckup(-1);
			}				
			
			if (Mis->getTotalMissionTime() >= 50)
				Rov->setCurrentExitDayInCheckup(Mis->getCompletionDay() + Rov->getDaysInCheckup());
			else
				Rov->setCurrentExitDayInCheckup(-1);

			if (Mis->getFailureDay() < Mis->getCompletionDay())
				missions_InExecution.enqueue(Mis, -(Mis->getFailureDay() * 1000));
			else
				missions_InExecution.enqueue(Mis, -(Mis->getCompletionDay() * 1000 + Mis->getTotalMissionTime()));
		}
		else if (rovers_Waiting_Polar.peek(Rov))
		{
			missions_Waiting_Emergency.dequeue(Mis);
			rovers_Waiting_Polar.dequeue(Rov);
			Mis->setAssignedRover(Rov);
			Mis->setStatistics(currentDay);
			Rov->setCurrentMissionsBeforeCheckup(Rov->getCurrentMissionsBeforeCheckup() + 1);
			if (Rov->getCurrentExitDayInCheckup() != -1 && !(Rov->getCurrentExitDayInCheckup() <= currentDay))
			{
				Rov->setNumberOfDamage(Rov->getNumberOfDamage() + 1);
				Rov->setCurrentExitDayInCheckup(-1);
			}
			
			if (Mis->getTotalMissionTime() >= 50)
				Rov->setCurrentExitDayInCheckup(Mis->getCompletionDay() + Rov->getDaysInCheckup());
			else
				Rov->setCurrentExitDayInCheckup(-1);

			if (Mis->getFailureDay() < Mis->getCompletionDay())
				missions_InExecution.enqueue(Mis, -(Mis->getFailureDay() * 1000));
			else
				missions_InExecution.enqueue(Mis, -(Mis->getCompletionDay() * 1000 + Mis->getTotalMissionTime()));
		}
		else
			break;
	}

	while (missions_Waiting_Polar.peek(Mis))
	{
		if (rovers_Waiting_Polar.peek(Rov))
		{
			missions_Waiting_Polar.dequeue(Mis);
			rovers_Waiting_Polar.dequeue(Rov);
			Mis->setAssignedRover(Rov);
			Mis->setStatistics(currentDay);
			Rov->setCurrentMissionsBeforeCheckup(Rov->getCurrentMissionsBeforeCheckup() + 1);
			if (Rov->getCurrentExitDayInCheckup() != -1 && !(Rov->getCurrentExitDayInCheckup() <= currentDay))
			{
				Rov->setNumberOfDamage(Rov->getNumberOfDamage() + 1);
				Mis->setStatistics(currentDay);
				Rov->setCurrentExitDayInCheckup(-1);
			}

			if (Mis->getTotalMissionTime() >= 50)
				Rov->setCurrentExitDayInCheckup(Mis->getCompletionDay() + Rov->getDaysInCheckup());
			else
				Rov->setCurrentExitDayInCheckup(-1);

			if (Mis->getFailureDay() < Mis->getCompletionDay())
				missions_InExecution.enqueue(Mis, -(Mis->getFailureDay() * 1000));
			else
				missions_InExecution.enqueue(Mis, -(Mis->getCompletionDay() * 1000 + Mis->getTotalMissionTime()));
		}
		else
			break;
	}
}
#pragma endregion 
