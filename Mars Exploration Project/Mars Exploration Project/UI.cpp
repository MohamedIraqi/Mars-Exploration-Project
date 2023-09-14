#include "UI.h"

UI::UI()
{
	modeOfOperation = ModesOfOperation::Silent_Mode;
}

void UI::setModeOfOperation()
{
	int x;
	cin >> x;
	switch (x)
	{
	case 1:
		modeOfOperation = ModesOfOperation::Interactive_Mode;
		break;
	case 2:
		modeOfOperation = ModesOfOperation::Step_By_Step_Mode;
		break;
	case 3:
		modeOfOperation = ModesOfOperation::Silent_Mode;
		break;
	default:
		break;
	}
}

ModesOfOperation UI::getModeOfOperation() const
{
	return modeOfOperation;
}

string UI::getUserInput() const
{
	string fileName;
	cin >> fileName;

	return fileName;
}

void UI::printString(string x) const
{
	cout << x << endl;
}
