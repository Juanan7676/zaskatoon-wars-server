#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "read.h"
int util::leer(SOCKET s, char *recvbuf)
	{
		SOCKET clisock=s;
		int iResult=0;
		int recvbuflen=255;
		iResult=recv(clisock,recvbuf,recvbuflen,0);
		if (iResult==SOCKET_ERROR) 
		{
			if (WSAGetLastError()==WSAETIMEDOUT) 
			{
				std::cout << "Timeout. More than 10 seconds passed since last receive block was received. Disconnecting." << std::endl;
				closesocket(clisock);
				return 1;
			}
			else 
			{
				if (WSAGetLastError()==10053L) 
				{
					std::cout << "The client closed the connection unexpectedly (without sending 'BYE'). Disconnecting." << std::endl;
					closesocket(clisock);
					return 1;
				}
				std::cout << "An error ocurred while receiving data from client:" << WSAGetLastError() << ". Disconnecting." << std::endl;
				closesocket(clisock);
				return 1;
			}
		}
		return 0;
	}