#include "BUILD.h"
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
#include <sstream>
#include <string>
#include "..\Util\metadataseparator.h"
#include "..\Proccessors\taskproc.h"
#include "..\Util\LettertoNumber.h"

void tasks::ProccessBuild(int cityID,std::string field,int TaskID)
{
	sql::Driver *driver;
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *rst;
	driver = sql::mysql::get_mysql_driver_instance();
	conn = driver->connect("localhost","root","power500");
	conn->setSchema("wars");
	stmt = conn->createStatement();
	std::stringstream comando;
	char *var1 = new char[2];
	memcpy(var1,field.c_str(),field.size());
	comando << "SELECT * FROM city" << cityID << " WHERE FieldX=" << util::lton(var1[0]) << " AND FieldY=" << var1[1];
	rst = stmt->executeQuery(comando.str()); rst->first();
	std::string Metadata = rst->getString("Metadata");
	char *cadena = new char[255];
	memcpy(cadena,Metadata.c_str(),Metadata.size());
	Tag var3 = util::SeparateTags(cadena,0);
	Tag var2 = util::SeparateTags(cadena,1);
	if (var2.TagValue == "1")
	{
		//Construction complete, delete task
		comando.str("");
		comando << "DELETE FROM tasks WHERE TaskID=" << TaskID;
		stmt->executeUpdate(comando.str());
		//Replace metadata with new building
		std::string nmetadata; std::stringstream taskmetadata;
		if (var3.TagValue == "BOMBA_DE_AGUA") 
		{
			nmetadata = "Type=BOMBA_DE_AGUA;Size=1;Remaining=10000;MJStored=0;";
			taskmetadata << "Type=Proccess;City=" << cityID << ";Field=" << field << ";";
		}
		if (var3.TagValue == "PERFORADORA_PETROLEO") nmetadata ="Type=PERFORADORA_PETROLEO;Size=1;Remaining=100000;MJStored=0;";
		if (var3.TagValue == "EXTRACCION_GAS") nmetadata = "Type=EXTRACCION_GAS;Size=1;Remaining=100000;MJStored=0;";
		if (var3.TagValue == "MINA_COBRE") nmetadata = "Type=MINA_COBRE;Size=1;Remaining=750000;MJStored=0;";
		if (var3.TagValue == "MINA_HIERRO") nmetadata = "Type=MINA_HIERRO;Size=1;Remaining=750000;MJStored=0;";
		/*
			TODO: More buildings...
		*/
		comando.str("");
		comando << "UPDATE city" << cityID << " SET Metadata='" << nmetadata << "' WHERE FieldX=" << util::lton(var1[0]) << " AND FieldY=" << var1[1];
		stmt->executeUpdate(comando.str());
		// Create new Task to proccess building
		Task *newtask = new Task(taskmetadata.str());
		newtask->push();
		delete newtask;
	}
	else
	{
		std::stringstream compose;
		int time = atoi(var2.TagValue.c_str());
		compose << "Building=" << var3.TagValue << ";RemainingTime=" << time - 1;
		comando.str("");
		comando << "UPDATE city" << cityID << " SET Metadata='" << compose.str() << "' WHERE FieldX=" << util::lton(var1[0]) << " AND FieldY=" << var1[1];
	}
	delete var1;
	delete cadena;
}
