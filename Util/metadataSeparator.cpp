#include "metadataseparator.h"
#include <sstream>

Tag util::SeparateTags(char *metadata,int index)
{
	char caracter = '\0'; int k=0; int StartTagName,EndTagName,StartTagValue,EndTagValue=0;
	for (int word=0;word <= index; word++)
	{
		StartTagName=k;
		do
		{
			caracter = metadata[k];
			k++;
			if (k >= sizeof(*metadata)) throw 1;
		} while (caracter != '=');
		EndTagName=k;
		StartTagValue = k + 1;
		k++;
	}
	//All right, found the desired TagName. Let's find the end of the TagValue.
	do
	{
		k++;
		caracter = metadata[k];
		if (k >= sizeof(metadata)) throw 2;
	} while (caracter != ';');
	EndTagValue = k - 1;
	//Compose the return value
	std::stringstream TagName,TagValue;
	for (int pos=StartTagName;pos <= EndTagName;pos++)
	{
		TagName << metadata[pos];
	}
	for (int pos=StartTagValue; pos <= EndTagValue;pos++)
	{
		TagValue << metadata[pos];
	}
	//Return the Tag object
	Tag taggy;
	taggy.TagName = TagName.str();
	taggy.TagValue = TagValue.str();
	return taggy;
}