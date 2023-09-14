#include "Rover.h"

Rover::Rover(RoverTypes r_RoverType, int r_ID, int r_MissionsBeforeCheckup, int r_DaysInCheckup, int r_Speed)
    :roverType(r_RoverType), ID(r_ID), missionsBeforeCheckup(r_MissionsBeforeCheckup), daysInCheckup(r_DaysInCheckup), speed(r_Speed)
{
    numberOfDamage = 0;
    currentMissionsBeforeCheckup = 0;
    currentExitDayInCheckup = -1;
}

RoverTypes Rover::getRoverType() const
{
    return roverType;
}

int Rover::getID() const
{
    return ID;
}

int Rover::getMissionsBeforeCheckup() const
{
    return missionsBeforeCheckup;
}

int Rover::getDaysInCheckup() const
{
    return daysInCheckup;
}

int Rover::getSpeed() const
{
    return speed;
}

void Rover::setNumberOfDamage(int r_NumberOfDamage)
{
    numberOfDamage = r_NumberOfDamage;
}

int Rover::getNumberOfDamage() const
{
    return numberOfDamage;
}

void Rover::setCurrentMissionsBeforeCheckup(int r_CurrentMissionsBeforeCheckup)
{
    currentMissionsBeforeCheckup = r_CurrentMissionsBeforeCheckup;
}

int Rover::getCurrentMissionsBeforeCheckup() const
{
    return currentMissionsBeforeCheckup;
}

void Rover::setCurrentExitDayInCheckup(int r_CurrentExitDayInCheckup)
{
    currentExitDayInCheckup = r_CurrentExitDayInCheckup;
}

int Rover::getCurrentExitDayInCheckup() const
{
    return currentExitDayInCheckup;
}

