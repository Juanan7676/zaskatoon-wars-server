#include "metadataseparator.h"
#include <sstream>
#include <iostream>

Tag util::SeparateTags(char *metadata,int index)
{
	Tag taggy;
	std::stringstream TagName,TagValue;
	std::string tamanyo = metadata;
	char caracter = '\0'; size_t k=0; int StartTagName,EndTagName,StartTagValue,EndTagValue=0;
	for (int word=0;word <= index; word++)
	{
		StartTagName=k;
		do
		{
			k++;
			if (k >= tamanyo.size()) goto fail;
		} while (metadata[k] != '=');
		EndTagName=k - 1;
		StartTagValue = k + 1;
		k++;
		do
		{
			caracter = metadata[k];
			k++;
			if (k >= tamanyo.size()) goto fail;
		} while (caracter != ';');
	EndTagValue = k - 2;
	}
	
	//Compose the return value
	for (int pos=StartTagName;pos <= EndTagName;pos++)
	{
		TagName << metadata[pos];
	}
	for (int pos=StartTagValue; pos <= EndTagValue;pos++)
	{
		TagValue << metadata[pos];
	}
	//Return the Tag object
	taggy.TagName = TagName.str();
	taggy.TagValue = TagValue.str();
	return taggy;
fail:
	taggy.TagName = "#ERROR#";
	taggy.TagValue = "#ERROR#";
	return taggy;
}