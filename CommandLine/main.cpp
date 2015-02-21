#include "main.h"
#include <string>
#include <iostream>

unsigned __stdcall CommandLine::main(void* data)
{
	bool *simulate = (bool*)data;
	std::string command;
	while (1)
	{
		std::cout << "> "; 
		std::cin >> command;
		if (command == "SIMULATE") *simulate = true;
		command = "";
	}
}