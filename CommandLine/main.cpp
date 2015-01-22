#include "main.h"
#include <string>
#include <iostream>

unsigned __stdcall CommandLine::main(void* data)
{
	std::string command;
	while (1)
	{
		std::cout << "> "; 
		std::cin >> command;
		std::cout << command << std::endl;
		command = "";
	}
}