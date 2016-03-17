#include "..\Common\SQL.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include "..\Util\read.h"
namespace
{
	int run_GET_PRICE(SOCKET clisock,char * recvbuf)
	{
		char respuesta[3] = "OK";
		send(clisock,respuesta,sizeof(respuesta),0);
		RtlZeroMemory(recvbuf,255);
		util::leer(clisock,recvbuf);
		std::string var1 = recvbuf;
		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("localhost","root","power500");
		conn->setSchema("wars");
		stmt = conn->createStatement();
		std::stringstream var2;
		var2 << "SELECT * FROM prices WHERE Valor='" << var1 << "'";
		rst = stmt->executeQuery(var2.str());
		if (rst->rowsCount() == 0)
		{
			std::cout << "Client sent an unknown value after GET_PRICE: " << var1 << std::endl;
			char var3[20] = "ERROR_BAD_VALUE";
			send(clisock,var3,sizeof(var3),0);
		}
		else
		{
			//Money price.
			rst->first();
			int var4 = rst->getInt("Precio");
			std::stringstream var7;
			var7 << var4;
			std::string var6 = var7.str();
			char *var5 = new char[var6.size() + 1];
			var5[var6.size()] = 0;
			memcpy(var5,var6.c_str(),var6.size());
			send(clisock,var5,var6.size(),0);
			RtlZeroMemory(recvbuf,sizeof(recvbuf));
			//Copper price.
			int var8 = rst->getInt("cobre");
			var7.str(""); var7 << var8;
			char *preciocobre = new char[var7.str().size() + 1];
			preciocobre[var7.str().size()] = 0;
			memcpy(preciocobre,var7.str().c_str(),var7.str().size());
			send(clisock,preciocobre,var7.str().size(),0);
			int resultado = util::leer(clisock,recvbuf); if (resultado == 1) return 1;
			if (strcmp(recvbuf,"OK") != 0) return 2;
			//Iron price.
			int var9 = rst->getInt("hierro");
			var7.str(""); var7 << var9;
			char *precioiron = new char[var7.str().size() + 1];
			precioiron[var7.str().size()] = 0;
			memcpy(precioiron,var7.str().c_str(),var7.str().size());
			send(clisock,precioiron,var7.str().size(),0);
			util::leer(clisock,recvbuf);
			if (strcmp(recvbuf,"OK") != 0) return 2;
			//Hours price.
			int var10 = rst->getInt("time");
			var7.str(""); var7 << var10;
			char *preciohoras = new char[var7.str().size() + 1];
			preciohoras[var7.str().size()] = 0;
			memcpy(preciohoras,var7.str().c_str(),var7.str().size());
			send(clisock,preciohoras,var7.str().size(),0);
			//End.
			resultado = util::leer(clisock,recvbuf); if (resultado == 1) return 1;
			if (strcmp(recvbuf,"OK") != 0) std::cout << "WARNING! No OK was received after sending prices. Client could have crashed!" << std::endl;
			delete preciocobre;
			delete preciohoras;
			delete precioiron;
			delete var5;
		}
		return 0;
	}
}