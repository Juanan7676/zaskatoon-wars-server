#include "registro.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <process.h>
#include <sstream>
#include <string>
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
#include "CityCreation.h"
#define DEFAULT_BUFLEN 512
using namespace std;
char recvbuff[256];
int leer(SOCKET s)
{
	SOCKET clisock=s;
	int iResult;
	int recvbuflen=DEFAULT_BUFLEN;
	iResult=recv(clisock,recvbuff,recvbuflen,0);
		if (iResult==SOCKET_ERROR) 
		{
			if (WSAGetLastError()==WSAETIMEDOUT) 
			{
				cout << "Timeout. More than 10 seconds passed since last receive block was received. Disconnecting." << endl;
				closesocket(clisock);
				return 1;
			}
			else 
			{
				if (WSAGetLastError()==10053L) 
				{
					cout << "The client closed the connection unexpectedly (without sending 'BYE'). Disconnecting." << endl;
					closesocket(clisock);
					return 1;
				}
				cout << "An error ocurred while receiving data from client:" << WSAGetLastError() << ". Disconnecting." << endl;
				closesocket(clisock);
				return 1;
			}
		}
	return 0;
}
int registro::registre(SOCKET s)
{
	SOCKET clisock=s;
	try 
	{
		char buff[256]="OK";
		int milliseconds=10000;
		int iResult;
		send(clisock,buff,sizeof(buff),0);
		setsockopt(clisock,SOL_SOCKET,SO_RCVTIMEO,(char *)&milliseconds,sizeof(milliseconds));
		//Username
		iResult=leer(clisock); if (iResult == 1) {return 1;}
		string user=recvbuff;
		ZeroMemory(&recvbuff,sizeof(recvbuff));
		//Password
		iResult=leer(clisock); if (iResult == 1) {return 1;}
		string pass=recvbuff;
		ZeroMemory(&recvbuff,sizeof(recvbuff));
		//IP-Register
		iResult=leer(clisock); if (iResult == 1) {return 1;}
		string ip=recvbuff;
		ZeroMemory(&recvbuff,sizeof(recvbuff));
		//E-mail
		iResult=leer(clisock); if (iResult == 1) {return 1;}
		string datos=recvbuff;
		//Start MySQL registering
		bool email;
		if (datos=="END") {email=false;}
		else {email=true;string emailstr=datos;}
		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		sql::ResultSet *rstt;
		driver=sql::mysql::get_mysql_driver_instance();
		conn=driver->connect("localhost","root","power500");
		conn->setSchema("wars");
		stmt=conn->createStatement();
		//Firstly... Are we sure there isn't an IP registered there? :/
		//EDIT: IF THE IP CHECKING ISN'T ENABLED?????????????????
		stringstream command;
		command << "SELECT * FROM ips WHERE IPAddress='" << ip << "'";
		rst=stmt->executeQuery(command.str());
		rstt=stmt->executeQuery("SELECT * FROM options");
		rstt->first();
		if (rstt->getInt("CheckIPAddresses") == 0) goto seguir_sin_registrar;
		bool registerednotlimit = false;
		if (rst->rowsCount()==0) goto seguir;
		else
		{
			rst->first();
			if (rst->getInt("NumberOfAccounts") >= rstt->getInt("LimitIPAddresses"))
			{
				cout << "User " << user << " tried to register with IP " << ip << ". Maybe it has a modified client?" << endl;
				char aka[255]="ERROR_BAD_IP";
				send(clisock,aka,sizeof(aka),0);
				closesocket(clisock);
				return 1;
			}
			else {
				registerednotlimit=true;
				goto seguir;
			}
		}
seguir:
		//Okay, that was great. Now, let's register the IP.
		if (registerednotlimit==false)
		{
			command.str("");
			command << "INSERT INTO ips VALUES ('" << ip << "',1)";
			stmt->executeUpdate(command.str());
		}
		else
		{
			command.str("");
			command << "UPDATE ips SET NumberOfAccounts=NumberOfAccounts+1 WHERE IPAddress='" << ip << "'";
			stmt->executeUpdate(command.str());
		}
seguir_sin_registrar:
		// The registerment process itself.
		rst=stmt->executeQuery("SELECT userID FROM users ORDER BY userID DESC");
		int last;
		if (rst->rowsCount()==0) last = 0;
		else
		{
			rst->first();
			last = rst->getInt("UserID");
		}
		stringstream stmtstr;
		stmtstr << "INSERT INTO users VALUES (" << last + 1 << ",'" << user << "','" << pass << "','user','23000000',NULL)";
		/* Master line. */stmt->executeUpdate(stmtstr.str());
		//Send an OK (all went fine)
		char buff2[256]="OK";
		send(clisock,buff2,sizeof(buff),0);
		iResult=CityCreation::CrearCiudad(user);
		if (iResult==0) {return 0;}
		else {return 1;}
	} catch (exception var) 
	{
		std::cout << "A generic error ocurred while registering client:" << var.what() << endl;
		closesocket(clisock);
		return 1;
	}
	return 0;
}