#include "..\Common\SQL.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include "..\Client.h"
#include "..\SendCityData.h"
#include "..\Util\read.h"
namespace
{
	int run_GET_INDEX(SOCKET clisock,char * recvbuf, Client * c)
	{
		try
		{
			char buffer[3] = "OK";
			send(clisock,buffer,sizeof(buffer),0);
			ZeroMemory(recvbuf,255);
			util::leer(clisock,recvbuf);
			std::string username = recvbuf;
			std::cout << username << std::endl;
			ZeroMemory(recvbuf,255);
			util::leer(clisock,recvbuf);
			std::string pasword = recvbuf;
			sql::Driver *driver;
			sql::Connection *conn;
			sql::Statement *stmt;
			sql::ResultSet *rst;
			driver = sql::mysql::get_mysql_driver_instance();
			conn = driver->connect("localhost","root","power500");
			conn->setSchema("wars");
			stmt = conn->createStatement();
			std::stringstream comando;
			comando << "SELECT * FROM users WHERE userName='" << username << "' AND pass='" << pasword << "'";
			rst = stmt->executeQuery(comando.str());
			if (rst->rowsCount()==0)
			{
				char badlogin[16] = "ERROR_BAD_LOGIN";
				send(clisock,badlogin,sizeof(badlogin),0);
				return 1;
			}
			else
			{
				char loginOK[255] = "OK";
				send(clisock,loginOK,sizeof(loginOK),0);
				c->setLogged(true);
				c->setUsername(username);
				c->setPassword(pasword);
				comando.str("");
				comando << "SELECT * FROM users WHERE userName='" << username << "'";
				rst = stmt->executeQuery(comando.str());
				comando.str("");
				rst->first();
				c->setCurrentCityID(rst->getInt("DefaultCity"));
			}
			bool Resultado=CityManagement::SendData(username,clisock);
			if (!Resultado)
			{
				return 1;
			}
			char enviaelfinal[5] = "END";
			send(clisock,enviaelfinal,sizeof(enviaelfinal),0);
			RtlZeroMemory(recvbuf,sizeof(recvbuf));
			int okvar = util::leer(clisock,recvbuf);
			if (strcmp(recvbuf,"OK") != 0 || okvar == 1)
			{
				std::cout << "Client didn't send the OK after sending all the city data. Closing connection.";
				return 1;
			}
			ZeroMemory(recvbuf,sizeof(recvbuf));
		}
		catch (sql::SQLException e)
		{
			std::cerr << e.what();
			return 1;
		}
		catch (std::exception e)
		{
			std::cerr << e.what();
			return 1;
		}
		return 0;
	}
}