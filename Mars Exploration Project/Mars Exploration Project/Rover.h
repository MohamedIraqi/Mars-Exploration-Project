#pragma once

#include "Definitions.h"

class Rover
{
private:
    const RoverTypes roverType;
    const int ID;
    const int missionsBeforeCheckup; //no.of missions before check up and the checkup durations
    const int daysInCheckup;// The duration (in days) of checkups that a rover needs to perform after completing N missions.
    const int speed;// in kilometers/hour.
    int numberOfDamage;// How many times the rover was used while in maintainance
    int currentMissionsBeforeCheckup; //Number of missions currently done by the rover since last checkup
    int currentExitDayInCheckup; //Day when the rover will be release from checkup
public:
    Rover(RoverTypes r_RoverType, int r_ID, int r_MissionsBeforeCheckup, int r_DaysInCheckup, int r_Speed);
    RoverTypes getRoverType() const;
    int getMissionsBeforeCheckup() const;
    int getDaysInCheckup() const;
    int getID() const;
    int getSpeed() const;
    void setNumberOfDamage(int r_NumberOfDamage);
    int getNumberOfDamage() const;
    void setCurrentMissionsBeforeCheckup(int r_CurrentMissionsBeforeCheckup);
    int getCurrentMissionsBeforeCheckup() const;
    void setCurrentExitDayInCheckup(int r_CurrentExitDayInCheckup);
    int getCurrentExitDayInCheckup() const;
};
