#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
namespace CityManagement
{
	bool SendData(std::string user,SOCKET s);
}