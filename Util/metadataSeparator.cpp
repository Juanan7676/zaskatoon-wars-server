#include "metadataseparator.h"
#include <sstream>
#include <iostream>

Tag util::SeparateTags(char *metadata,int index)
{
	try
	{
	std::string tamanyo = metadata;
	char caracter = '\0'; size_t k=0; int StartTagName,EndTagName,StartTagValue,EndTagValue=0;
	for (int word=0;word <= index; word++)
	{
		StartTagName=k;
		do
		{
			k++;
			if (k >= tamanyo.size()) throw 1;
		} while (metadata[k] != '=');
		EndTagName=k - 1;
		StartTagValue = k + 1;
		k++;
		do
		{
			caracter = metadata[k];
			k++;
			if (k >= tamanyo.size()) throw 2;
		} while (caracter != ';');
	EndTagValue = k - 2;
	}
	
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
	catch (std::exception e)
	{
		std::cerr << "Error thrown at metadataseparator.cpp" << std::endl;
		Tag taggy;
		return taggy;
	}
}