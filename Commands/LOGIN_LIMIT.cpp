#include "..\Common\SQL.h"
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