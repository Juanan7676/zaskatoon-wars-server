#define WIN32_LEAN_AND_MEAN
#include <string>
#include "SendCityData.h"
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
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
char buffer[255];
int leerr2(SOCKET s)
{
	std::cout << "Hola";
	SOCKET clisock=s;
	int iResult;
	int bufferlen=sizeof(buffer);
	iResult=recv(clisock,buffer,bufferlen,0);
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

bool CityManagement::SendData(std::string user,SOCKET s)
{
	try 
	{
		SOCKET clisock = s;
		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("localhost","root","power500");
		conn->setSchema("wars");
		stmt = conn->createStatement();
		std::stringstream comando;
		std::cout << "Sending city for " << user << std::endl;
		comando << "SELECT * FROM users WHERE userName='" << user << "'";
		rst = stmt->executeQuery(comando.str());
		comando.str("");
		rst->first();
		int CityID = rst->getInt("DefaultCity");
		comando << "SELECT * FROM city" << CityID;
		rst = stmt->executeQuery(comando.str());
		rst->first();
		std::stringstream casillas;
		std::string letra;
		do
		{
			switch (rst->getInt("FieldX"))
			{
			case 1:
				letra = "A";
				break;
			case 2:
				letra = "B";
				break;
			case 3:
				letra = "C";
				break;
			case 4:
				letra = "D";
				break;
			case 5:
				letra = "E";
				break;
			case 6:
				letra = "F";
				break;
			case 7:
				letra = "G";
				break;
			case 8:
				letra = "H";
				break;
			case 9:
				letra = "I";
				break;
			case 10:
				letra = "J";
				break;
			case 11:
				letra = "K";
				break;
			case 12:
				letra = "L";
				break;
			case 13:
				letra = "M";
				break;
			case 14:
				letra = "N";
				break;
			case 15:
				letra = "O";
				break;
			case 16:
				letra = "P";
				break;
			case 17:
				letra = "Q";
				break;
			case 18:
				letra = "R";
				break;
			case 19:
				letra = "S";
				break;
			case 20:
				letra = "T";
				break;
			default:
				return false;
			}
			casillas << letra << rst->getInt("FieldY");
			std::string var3 = casillas.str();
			const char *casilla = var3.c_str();
			send(clisock,casilla,sizeof(casilla),0);
			casillas.str("");
			int var1 = leerr2(clisock);
			if (strcmp(buffer,"OK") != 0 || var1 != 0)
			{
				std::cout << "Client didn't sent an OK after sending field number. Closing connection" << std::endl;
				return false;
			}
			std::string tipodecasilla;
			if (rst->getString("Type") == "CAMPO")
			{
				tipodecasilla = "GRASS";
			}
			else if (rst->getString("Type") == "PIEDRAS")
			{
				tipodecasilla = "ROCKS";
			}
			else if (rst->getString("Type") == "OIL")
			{
				tipodecasilla = "OIL";
			}
			else if (rst->getString("Type") == "AYUNTAMIENTO")
			{
				tipodecasilla = "MAIN";
			}
			else if (rst->getString("Type") == "GAS")
			{
				tipodecasilla = "GAS";
			}
			else if (rst->getString("Type") == "AGUA")
			{
				tipodecasilla = "WATER";
			}
			else if (rst->getString("Type") == "LAVA")
			{
				tipodecasilla = "LAVA";
			}
			else if (rst->getString("Type") == "URANIO")
			{
				tipodecasilla = "URAN";
			}
			else if (rst->getString("Type") == "COAL")
			{
				tipodecasilla = "COAL";
			}
			else if (rst->getString("Type") == "HIERRO")
			{
				tipodecasilla = "IRON";
			}
			else if (rst->getString("Type") == "COBRE")
			{
				tipodecasilla = "COPPER";
			}
			else if (rst->getString("Type") == "PIEDRAS")
			{
				tipodecasilla = "ROCKS";
			}
			else if (rst->getString("Type") == "EDIFICIO")
			{
				tipodecasilla = "BUILD";
			}
			else if (rst->getString("Type") == "CULTIVO")
			{
				tipodecasilla = "PLANT";
			}
			else if (rst->getString("Type") == "CENTRAL_NUCLEAR")
			{
				tipodecasilla = "NUKE";
			}
			else if (rst->getString("Type") == "ARRASADO")
			{
				tipodecasilla = "DESTR";
			}
			else if (rst->getString("Type") == "RADIACION")
			{
				tipodecasilla = "RAD";
			}
			else if (rst->getString("Type") == "ESCUDO")
			{
				tipodecasilla = "SHIELD";
			}
			else if (rst->getString("Type") == "TERRENO_FERTIL")
			{
				tipodecasilla = "FERT";
			}
			const char *tipodecasillabuff = tipodecasilla.c_str();
			send(clisock,tipodecasillabuff,sizeof(tipodecasillabuff),0);
			RtlZeroMemory(buffer,sizeof(buffer));
			int var2 = leerr2(clisock);
			if (strcmp(buffer,"OK") != 0 || var2 != 0)
			{
				std::cout << "Client didn't send an OK after sending field type. Closing connection." << std::endl;
				return false;
			}
		} while (rst->next());
		return true;
	} 
	catch (sql::SQLException e)
	{
		std::cerr << e.what();
		return false;
	} 
	catch (std::exception e)
	{
		std::cerr << e.what();
		return false;
	}
}
