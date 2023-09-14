#pragma once

#include "Definitions.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class UI
{
private:
	ModesOfOperation modeOfOperation;
public:
	UI();
	void setModeOfOperation();
	ModesOfOperation getModeOfOperation() const;
	string getUserInput() const;
	void printString(string output) const;
};
