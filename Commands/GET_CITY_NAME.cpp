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
#include "..\Util\read.h"
namespace
{
	int run_GET_CITY_NAME(SOCKET clisock,Client c,char * recvbuf)
	{
		if (c.isLogged() == false)
		{
			char buffer[20] = "ERROR_NOT_LOGGED";
			send(clisock,buffer,sizeof(buffer),0);
			return 0;
		}
		char ok[3] = "OK";
		send(clisock,ok,sizeof(ok),0);
		int CityID = c.getCurrentCityID();
		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("localhost","root","power500");
		conn->setSchema("wars");
		stmt = conn->createStatement();
		std::stringstream var1;
		int resultado = util::leer(clisock,recvbuf); if (resultado != 0) return 1;
		std::string cityID = recvbuf;
		var1 << "SELECT cityName FROM cities WHERE CityID='" << cityID << "'";
		rst = stmt->executeQuery(var1.str());
		if (rst->rowsCount() == 0)
		{
			char error[21] = "ERROR_INVALID_CITYID";
			send(clisock,error,sizeof(error),0);
			return 0;
		}
		rst->first();
		std::string nombre = rst->getString("cityName");
		char *enviar = new char[nombre.size() + 1];
		enviar[nombre.size()] = 0;
		memcpy(enviar,nombre.c_str(),nombre.size());
		send(clisock,enviar,nombre.size(),0);
		RtlZeroMemory(recvbuf,sizeof(recvbuf));
		util::leer(clisock,recvbuf);
		if (strcmp(recvbuf,"OK") != 0)
		{
			std::cout << "WARNING! Client didn't send an ok after sending City Name in GET_CITY_NAME. Could have crashed or disconnected." << std::endl;
		}
		delete enviar;
		return 0;
	}
}