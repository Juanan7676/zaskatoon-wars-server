#include "CitySimylator.h"
#include <time.h>
#include "windows.h"

unsigned __stdcall CitySimulator::StartThread(void* data)
{
	time_t stamp = time(NULL);
	struct tm* tiempo = localtime(&stamp);
	while(1) //Must add a "stop simulation" and command-line in main thread. Pointer to a bool variable (data) and when it is true, exit loop.
	{
		int minuto;
		do
		{
			Sleep(1000);
			minuto = tiempo->tm_min;
		} while (minuto != 0); //Check for next update
	}
	return 0;
}
