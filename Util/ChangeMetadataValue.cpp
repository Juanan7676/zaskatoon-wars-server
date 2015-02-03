#include "ChangeMetadataValue.h"
#include <sstream>

void util::changemetadata(char* cadena, TagList tags)
{
	std::stringstream scadena;
	for (size_t k=0;k <= tags.size(); ++k)
	{
		if (k != 0) scadena << ";";
		scadena << tags[k].TagName << "=";
		scadena << tags[k].TagValue;
	}
	scadena << ";";
	memcpy(cadena,scadena.str().c_str(),scadena.str().size());
}