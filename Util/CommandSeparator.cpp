#include "CommandSeparator.h"
#include <sstream>

std::string util::separatewords(std::string command, int index)
{
	char *cadena = new char[command.size() + 1];
	int k=0;
	int Start=-1;
	int Finish;
	for (int lazo=0;lazo <= index; ++lazo)
	{
		Start = k + 1;
		do
		{
			k++;
		} while (cadena[k] != ' ');
		Finish = k - 1;
	}
	//Compose string
	std::stringstream compose;
	for (int n=Start;n <= Finish; ++n)
	{
		compose << cadena[n];
	}
	delete cadena;
	return compose.str();
}