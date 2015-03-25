#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include "..\Proccessors\taskproc.h"
#include "..\Proccessors\BUILD.h"
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
		if (!c.isLogged())
		{
			char error[17] = "ERROR_NOT_LOGGED";
			send(clisock,error,17,0);
			return 1;
		}
		else
		{
			char ok[3] = "OK";
			send(clisock,ok,3,0);
		}
		int iResult = util::leer(clisock,recvbuf);
		if (iResult != 0) return 1;
		std::string comando = recvbuf;
		//Divide in words
		std::string Word1 = util::separatewords(comando,0);
		std::string Word2 = util::separatewords(comando,1);
		std::string Word3 = util::separatewords(comando,2);
		std::string Word4 = util::separatewords(comando,3);
		std::string Word5 = util::separatewords(comando,4);
		if (Word2 == "BUILD") tasks::CreateBuild(Word1,Word2,Word3,Word4,Word5,c);
		/*
		TODO : More tasks
		*/
		char ok[3] = "OK";
		send(clisock,ok,3,0);
		return 0;
	}
}