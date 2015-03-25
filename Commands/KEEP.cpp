#include "..\Common\SQL.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
namespace
{
	int run_KEEP(SOCKET * socket,int *milliseconds)
	{
		*milliseconds=100000000;
		setsockopt(*socket,SOL_SOCKET,SO_RCVTIMEO,(char *)milliseconds,9);
		char sendbuffer[5] = "OK";
		try
		{
			send(*socket,sendbuffer,sizeof(sendbuffer),0);
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
			closesocket(*socket);
			return 1;
		}
		return 0;
	}
}