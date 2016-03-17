#include "..\Common\SQL.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\Util\read.h"
#include <string>
namespace
{
	int run_NAME_AVAIABLE(SOCKET clisock,char * recvbuf)
	{
		char buff[255]="OK";
		send(clisock,buff,sizeof(buff),0);
		ZeroMemory(recvbuf,255);
		int iResult=util::leer(clisock,recvbuf); if (iResult==1) return 1;
		std::string datos=recvbuf;
		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		driver=sql::mysql::get_mysql_driver_instance();
		conn=driver->connect("localhost","root","power500");
		conn->setSchema("wars");
		stmt=conn->createStatement();
		rst=stmt->executeQuery("SELECT * FROM users");
		rst->first();
		bool used=false;
		std::string response;
		if (rst->rowsCount()!=0)
		{
			do
			{
				if (rst->getString("userName")==datos) used=true;
			} while(rst->next());
		}
		if (used==true) response="NO";
		else {response="YES";}
		char *bufff=new char[response.size()+1];
		bufff[response.size()]=0;
		memcpy(bufff,response.c_str(),response.size());
		send(clisock,bufff,sizeof(bufff),0);
		return 0;
	}
}