#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "..\Proccessors\taskproc.h"
#include "..\Util\read.h"

namespace
{
	int run_CREATE_TASK(SOCKET clisock,char *recvbuf)
	{
		memset(recvbuf,0,255);
		char ok[3] = "OK";
		send(clisock,ok,3,0);
		int iResult = util::leer(clisock,recvbuf);
		if (iResult != 0) return 1;
		std::string comando = recvbuf;
		return 0;
	}
}