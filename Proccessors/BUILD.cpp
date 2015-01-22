#include "BUILD.h"
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
#include <sstream>
#include <string>
#include "..\Util\metadataseparator.h"

void tasks::ProccessBuild(int cityID,std::string field)
{
	sql::Driver *driver;
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *rst;
	driver = sql::mysql::get_mysql_driver_instance();
	conn = driver->connect("localhost","root","power500");
	conn->setSchema("wars");
	std::stringstream comando;
	char *var1 = new char[2];
	memcpy(var1,field.c_str(),field.size());
	comando << "SELECT * FROM city" << cityID << " WHERE FieldX=" << var1[0] << " AND FieldY=" << var1[1];
	rst = stmt->executeQuery(comando.str()); rst->first();
	std::string Metadata = rst->getString("Metadata");
	char *cadena = new char[255];
	memcpy(cadena,Metadata.c_str(),Metadata.size());
	Tag var3 = util::SeparateTags(cadena,0);
	Tag var2 = util::SeparateTags(cadena,1);
	if (var2.TagValue == "1")
	{
		//Code to build the city
	}
	else
	{
		std::stringstream compose;
		int time = atoi(var2.TagValue.c_str());
		compose << "Building=" << var3.TagValue << ";RemainingTime=" << time - 1;
		comando.str("");
		comando << "UPDATE city" << cityID << " SET Metadata='" << compose.str() << "' WHERE FieldX=" << var1[0] << " AND FieldY=" << var1[1];
	}
}
