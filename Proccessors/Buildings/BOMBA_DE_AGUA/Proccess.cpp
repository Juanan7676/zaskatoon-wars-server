#include "Proccess.h"
#include <cppconn\prepared_statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn\driver.h>
#include <cppconn\connection.h>
#include <cppconn\build_config.h>
#include <cppconn\config.h>
#include <cppconn\datatype.h>
#include <cppconn\statement.h>
#include <cppconn\resultset.h>
#include <cppconn\exception.h>
#include <cppconn\prepared_statement.h>
#include "..\..\..\Util\metadataseparator.h"

void tasks::buildings::proccess_BOMBA_DE_AGUA(int CityID,int FieldX,int FieldY,std::string metadata)
{
	char *metadatac = new char[metadata.size()];
	memcpy(metadatac,metadata.c_str(),metadata.size());
	Tag size = util::SeparateTags(metadatac,1);
	int sacarAgua = atoi(size.TagValue.c_str()) * 100;
	int energiaNecesaria = sacarAgua * 50;
	Tag currentMJ = util::SeparateTags(metadatac,4);
	if (atoi(currentMJ.TagValue.c_str()) < energiaNecesaria)
	{
		//Not enough energy to perform operation
		return;
	}
}