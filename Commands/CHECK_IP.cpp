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
#include "..\Util\read.h"
namespace
{

	int run_CHECK_IP(SOCKET clisock, char* recvbuf)
	{
		char buff[3]="OK";
		send(clisock,buff,sizeof(buff),0);
		ZeroMemory(&recvbuf,sizeof(recvbuf));
		int resultado = util::leer(clisock,recvbuf);  if (resultado != 0) return 1;
		std::string datos=recvbuf;
		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		driver=sql::mysql::get_mysql_driver_instance();
		conn=driver->connect("localhost","root","power500");
		conn->setSchema("wars");
		stmt=conn->createStatement();
		rst=stmt->executeQuery("SELECT * FROM ips");
		rst->first();
		bool used=false;
		int ipnumber=0;
		if (rst->rowsCount()==0) goto salto;
		do
		{

			if (rst->getString("IPAddress").compare(datos))
			{
				used=true;
				ipnumber=rst->getInt("NumberOfAccounts");
				break;
			}
		} while (rst->next());
salto:
		if (used==false)
		{
			char buff[4]="YES";
			send(clisock,buff,sizeof(buff),0);
		}
		if (used==true)
		{
			rst=stmt->executeQuery("SELECT * FROM options");
			rst->first();
			int limite;
			limite=rst->getInt("LimitIpAddresses");
			if (ipnumber >= limite)
			{
				char buff[3]="NO";
				send(clisock,buff,sizeof(buff),0);
			}
			else
			{
				char buff[4]="YES";
				send(clisock,buff,sizeof(buff),0);
			}
		}
		return 0;
	}
}