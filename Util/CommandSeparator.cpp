#include "CommandSeparator.h"
#include <sstream>

std::string util::separatewords(std::string command, int index)
{
	char *cadena = new char[command.size() + 1];
	int k=-1;
	int Start;
	int Finish;
	for (int lazo=0;lazo <= index; ++lazo)
	{
		Start = k + 1;
		do
		{
			k++;
			if (k > command.size()) return "#ERROR#";
		} while (cadena[k] != ' ' && cadena[k] != ';');
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