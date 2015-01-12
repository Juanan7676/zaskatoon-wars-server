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
namespace
{
	int run_KEEP(SOCKET * socket)
	{
		int milliseconds=100000000;
		setsockopt(*socket,SOL_SOCKET,SO_RCVTIMEO,(char *)&milliseconds,sizeof(milliseconds));
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