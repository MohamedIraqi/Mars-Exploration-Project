#include "MarsStation.h"
#include "UI.h"

int main()
{
	UI Interface;
	MarsStation Station(&Interface);

	Station.execute();

	return 0;
}