#define WIN32_LEAN_AND_MEAN
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma once

class Client
{
private:
	SOCKET s;
	bool logged;
	std::string username;
	std::string password;
	int CurrentCityID;
public:
	Client(SOCKET s);
	SOCKET getSocket();
	bool isLogged();
	void setLogged(bool opt);
	std::string getUsername();
	void setUsername(std::string username);
	std::string getPassword();
	void setPassword(std::string password);
	void setCurrentCityID(int city);
	int getCurrentCityID();
};
