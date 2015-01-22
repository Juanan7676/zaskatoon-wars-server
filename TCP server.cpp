#define WIN32_LEAN_AND_MEAN
#include "registro.h"
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
#include "SendCityData.h"
#include "Client.h"
#include "CitySimylator.h"
#include "Commands\LOGIN_LIMIT.cpp"
#include "Commands\CHECK_IP.cpp"
#include "Commands\NAME_AVAIABLE.cpp"
#include "Commands\KEEP.cpp"
#include "Commands\GET_INDEX.cpp"
#include "Commands\GET_PRICE.cpp"
#include "Commands\GET_CITY_ID.cpp"
#include "Commands\GET_CITY_NAME.cpp"
#include "Util\read.h"
#include "CommandLine\main.h"
#undef UNICODE
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "25565"
#define NULL_CHAR '/0'
int recvbuflen = DEFAULT_BUFLEN;
int conn=0;
int cityID;
using namespace std;
unsigned __stdcall ClientSession(void* data)
{
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
    Client* data2 = (Client*)data;
	Client c = *data2;
	SOCKET clisock = c.getSocket();
	int milliseconds=10000;
	while(1) 
	{
	RtlZeroMemory(&recvbuf,sizeof(recvbuf));
	setsockopt(clisock,SOL_SOCKET,SO_RCVTIMEO,(char *)&milliseconds,sizeof(milliseconds));
	iResult = util::leer(clisock,recvbuf);
	if (iResult == 1) break;
	if (strcmp(recvbuf,"HI")==0) {
		char buff[DEFAULT_BUFLEN]="WELCOME";
		send(clisock,buff,sizeof(buff),0);
	}
	else if (strcmp(recvbuf,"BYE")==0) break;
	else if (strcmp(recvbuf,"LOGIN_LIMIT")==0) run_LOGIN_LIMIT(clisock);
	else if (strcmp(recvbuf,"CHECK_IP")==0) 
	{
		iResult = run_CHECK_IP(clisock,recvbuf);
		if (iResult == 1) break;
	}
	else if (strcmp(recvbuf,"REGISTER")==0)
	{
		iResult=registro::registre(clisock);
		if (iResult == 1) break;
	}
	else if (strcmp(recvbuf,"NAME_AVAIABLE")==0)
	{
		iResult = run_NAME_AVAIABLE(clisock,recvbuf);
		if (iResult == 1) break;
	}
	else if (strcmp(recvbuf,"KEEP")==0)
	{
		iResult = run_KEEP(&clisock,&milliseconds);
		if (iResult == 1) break;
	}
	else if (strcmp(recvbuf,"GET_INDEX")==0)
	{
		iResult = run_GET_INDEX(clisock,recvbuf,&c);
		if (iResult == 1) break;
	}
	else if (strcmp(recvbuf,"GET_PRICE")==0)
	{
		iResult = run_GET_PRICE(clisock,recvbuf);
		if (iResult == 1) break;
		if (iResult == 2) continue;
	}
	else if (strcmp(recvbuf,"GET_CITY_ID")==0)
	{
		iResult = run_GET_CITY_ID(clisock,c);
		if (iResult == 1) continue;
	}
	else if (strcmp(recvbuf,"GET_CITY_NAME") == 0)
	{
		iResult = run_GET_CITY_NAME(clisock,c,recvbuf);
		if (iResult != 0) break;
	}
	else {
		stringstream devolver;
		char buff[DEFAULT_BUFLEN]="Unknown command.";
		send(clisock,buff,sizeof(buff),0);
	}
	}
closesocket(clisock);
--conn;
cout << "Client disconnected/Ended communication with client. Current clients connected:" << conn << endl;
return 0;
}
int main() {
	unsigned CitySimulatorThread;
	HANDLE h = (HANDLE)_beginthreadex(NULL,0,&CitySimulator::StartThread,NULL,0,&CitySimulatorThread);
	WSADATA wsaData;
	unsigned CommandLineThread;
	HANDLE hh = (HANDLE)_beginthreadex(NULL,0,&CommandLine::main,NULL,0,&CommandLineThread);
    int iResult;
    SOCKET sock = INVALID_SOCKET;
    SOCKET clisock = INVALID_SOCKET;
	std::cout << "Inicializando WSA..." << endl;
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }
	std::cout << "WSA inicializado con éxito." << endl;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	std::cout << "Obteniendo direccion IP local..." << endl;
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
	std::cout << "Exito." << endl;
	std::cout << "Creando socket..." << endl;
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		exit(2);
	}
	std::cout << "Exito." << endl;
	std::cout << "Binding socket..." << endl;
	iResult = bind(sock, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(sock);
        WSACleanup();
        exit(3);
    }
	std::cout << "Exito." << endl;
	std::cout << "Escuchando conexiones entrantes..." << endl;
	freeaddrinfo(result);
	if ( listen( sock, SOMAXCONN ) == SOCKET_ERROR ) {
		printf( "Listen failed with error: %ld\n", WSAGetLastError() );
		closesocket(sock);
		WSACleanup();
		exit(4);
	}
	int maxconn=6;
	while(1) {
		clisock=accept(sock,NULL,NULL);
		if (clisock==INVALID_SOCKET) {
			printf("Accept failed with error: %ld\n",WSAGetLastError());
		}
		unsigned threadID;
		conn++;
		Client* c = new Client(clisock);
		cout << "Client connected. #" << conn << endl;
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &ClientSession,(void*)c, 0, &threadID);
		cout << "Current clients connected:" << conn << endl;
	}

}