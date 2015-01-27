#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include "..\Proccessors\taskproc.h"
#include "..\Util\read.h"
#include "..\Util\CommandSeparator.h"
#include "..\Util\metadataseparator.h"
#include "..\Util\LettertoNumber.h"
#include "..\Client.h"
#include <iostream>
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

namespace
{
	int run_CREATE_TASK(SOCKET clisock,char *recvbuf,Client c)
	{
		memset(recvbuf,0,255);
		char ok[3] = "OK";
		send(clisock,ok,3,0);
		int iResult = util::leer(clisock,recvbuf);
		if (iResult != 0) return 1;
		std::string comando = recvbuf;
		//Divide in words
		std::string Word1 = util::separatewords(comando,0);
		std::string Word2 = util::separatewords(comando,1);
		std::string Word3 = util::separatewords(comando,2);
		std::string Word4 = util::separatewords(comando,3);
		std::string Word5 = util::separatewords(comando,4);
		if (Word2 == "BUILD") //A build task. Yay!
		{
			char *metadatos = new char[255];
			memcpy(metadatos,Word3.c_str(),Word3.size());
			Tag building = util::SeparateTags(metadatos,0);
			std::stringstream var1;
			var1 << "Type=Build;City=" << c.getCurrentCityID() << ";Field=" << Word1 << ";";
			Task newtask = Task(var1.str());
			newtask.push();
			try
			{
				sql::Driver *driver;
				sql::Connection *conn;
				sql::Statement *stmt;
				sql::ResultSet *rst;
				driver = sql::mysql::get_mysql_driver_instance();
				conn = driver->connect("localhost","root","power500");
				conn->setSchema("wars");
				stmt = conn->createStatement();
				var1.str("");
				var1 << "SELECT * FROM prices WHERE Valor='" << building.TagValue << "'";
				rst = stmt->executeQuery(var1.str());
				rst->first();
				std::stringstream comando;
				char *var2 = new char[2];
				memcpy(var2,Word1.c_str(),2);
				comando << "UPDATE city" << c.getCurrentCityID() << " SET Metadata='Building=" << building.TagValue << ";RemainingTime=" << rst->getInt("time") << ";MJStored=0;MJRemaining=" << rst->getInt("MJ") << ";' WHERE FieldX=" << util::lton(var2[0]) << " AND FieldY=" << var2[1];
				stmt->executeUpdate(comando.str());
				comando.str("");
				comando << "UPDATE city" << c.getCurrentCityID() << " SET Type='EDIFICIO' WHERE FieldX=" << util::lton(var2[0]) << " AND FieldY=" << var2[1];
				stmt->executeUpdate(comando.str());
				delete var2;
			} catch (sql::SQLException e)
			{
				std::cerr << e.what() << std::endl;
			}
			delete metadatos;
		}
		/*
		TODO : More tasks
		*/
		send(clisock,ok,3,0);
		return 0;
	}
}