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
	void run_LOGIN_LIMIT(SOCKET sock)
	{
		sql::Driver *dr;
		sql::Connection *cnn;
		sql::Statement *stm;
		sql::ResultSet *rstt;
		dr=sql::mysql::get_mysql_driver_instance();
		cnn=dr->connect("localhost","root","power500");
		cnn->setSchema("wars");
		stm=cnn->createStatement();
		rstt=stm->executeQuery("SELECT * FROM options");
		rstt->first();
		int opt = rstt->getInt("CheckIPAddresses");
		if (opt==1) {
			char respuesta[4]="YES";
			send(sock,respuesta,sizeof(respuesta),0);
		}
		if (opt==0) {
			char respuesta[3]="NO";
			send(sock,respuesta,sizeof(respuesta),0);
		}
	}
}