#include "Client.h"

SOCKET Client::getSocket()
{
	return this->s;
}
bool Client::isLogged()
{
	return this->logged;
}
void Client::setLogged(bool opt)
{
	this->logged = opt;
}
std::string Client::getUsername()
{
	return this->username;
}
void Client::setUsername(std::string username)
{
	this->username = username;
}
std::string Client::getPassword()
{
	return this->password;
}
void Client::setPassword(std::string password)
{
	this->password = password;
}
Client::Client(SOCKET s)
{
	this->s = s;
	this->logged = false;
	this->username = "";
	this->password = "";
	this->CurrentCityID = 0;
}
void Client::setCurrentCityID(int city)
{
	this->CurrentCityID = city;
}
int Client::getCurrentCityID()
{
	return this->CurrentCityID;
}