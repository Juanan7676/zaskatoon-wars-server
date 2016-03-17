#pragma once
#include <string>
struct Tag
{
	std::string TagName;
	std::string TagValue;
};
namespace util
{
	Tag SeparateTags(char * metadata, int index);
}