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
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include "..\Client.h"
#include <string>
namespace
{
	int run_GET_CITY_ID(SOCKET clisock, Client c)
	{
		if (c.isLogged() == false)
		{
			char buffer[20] = "ERROR_NOT_LOGGED";
			send(clisock,buffer,sizeof(buffer),0);
			return 1;
		}
		char ok[3] = "OK";
		send(clisock,ok,sizeof(ok),0);
		int CityID = c.getCurrentCityID();
		std::stringstream var2;
		var2 << CityID;
		std::string var3 = var2.str();
		char *var1 = new char[var3.size()+1];
		var1[var3.size()] = 0;
		memcpy(var1,var3.c_str(),var3.size());
		send(clisock,var1,var3.size(),0);
		return 0;
	}
}