#include "BUILD.h"
#include "..\Common\SQL.h"
#include <sstream>
#include <string>
#include "..\Proccessors\taskproc.h"
#include "..\Util\LettertoNumber.h"
#include "..\Util\ChangeMetadataValue.h"

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
	char *cadena = new char[Metadata.size() + 1];
	memset(cadena,0,Metadata.size());
	memcpy(cadena,Metadata.c_str(),Metadata.size());
	cadena[Metadata.size()] = '\0';
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
		if (var3.TagValue == "BOMBA_DE_AGUA") nmetadata = "Type=BOMBA_DE_AGUA;Size=1;Remaining=10000;MJStored=0;StoredWater=0;";
		if (var3.TagValue == "PERFORADORA_PETROLEO") nmetadata ="Type=PERFORADORA_PETROLEO;Size=1;Remaining=100000;MJStored=0;StoredOil=0";
		if (var3.TagValue == "EXTRACCION_GAS") nmetadata = "Type=EXTRACCION_GAS;Size=1;Remaining=100000;MJStored=0;StoredGas=0";
		if (var3.TagValue == "MINA_COBRE") nmetadata = "Type=MINA_COBRE;Size=1;Remaining=750000;MJStored=0;StoredCopper=0";
		if (var3.TagValue == "MINA_HIERRO") nmetadata = "Type=MINA_HIERRO;Size=1;Remaining=750000;MJStored=0;StoredIron=0";
		if (var3.TagValue == "CENTRAL_HIDROELECTRICA") nmetadata = "Type=CENTRAL_HIDROELECTRICA;EffUpgrades=0;CapUpgrades=0;StoredWater=0;MJStored=0;";
		/*
			TODO: More buildings...
		*/
		taskmetadata << "Type=Proccess;Building=" << var3.TagValue << ";City=" << cityID << ";Field=" << field << ";";
		comando.str("");
		comando << "UPDATE city" << cityID << " SET Metadata='" << nmetadata << "' WHERE FieldX=" << util::lton(var1[0]) << " AND FieldY=" << var1[1];
		stmt->executeUpdate(comando.str());
		// Create new Task to proccess building
		Task *newtask = new Task(taskmetadata.str());
		newtask->push(0);
		delete newtask;
	}
	else
	{
		int time = atoi(var2.TagValue.c_str());
		time--;
		std::stringstream tmp;
		tmp << time;
		TagList t;
		Tag current;
		size_t k=0;
		while (1)
		{
			current = util::SeparateTags(cadena,k);
			if (current.TagValue == "#ERROR#") break;
			if (current.TagName == "RemainingTime") current.TagValue=tmp.str();
			t.push_back(current);
			k++;
		}
		util::changemetadata(cadena,t);
		std::string compose = cadena;
		comando.str("");
		comando << "UPDATE city" << cityID << " SET Metadata='" << compose << "' WHERE FieldX=" << util::lton(var1[0]) << " AND FieldY=" << var1[1];
		stmt->executeUpdate(comando.str());
	}
	delete[] var1;
	delete[] cadena;
}

void tasks::CreateBuild(std::string Word1,std::string Word2,std::string Word3,std::string Word4,std::string Word5,Client c)
{
	char *metadatos = new char[255];
			memcpy(metadatos,Word3.c_str(),Word3.size());
			Tag building = util::SeparateTags(metadatos,0);
			std::stringstream var1;
			var1 << "Type=Build;City=" << c.getCurrentCityID() << ";Field=" << Word1 << ";";
			Task newtask = Task(var1.str());
			newtask.push(0);
			try
			{
				sql::Driver *driver;
				sql::Connection *conn;
				sql::Statement *stmt;
				sql::ResultSet *rst;
				driver = sql::mysql::get_mysql_driver_instance();
				conn = driver->connect("localhost","root","power500");
				conn->setSchema("wars");
				stmt = conn->createStatement();
				var1.str("");
				if (building.TagValue == "MINA_HIERRO" || building.TagValue == "MINA_COBRE") 
				{
					std::string edificio = "MINA";
					var1 << "SELECT * FROM prices WHERE Valor='" << edificio << "'";
				}
				else var1 << "SELECT * FROM prices WHERE Valor='" << building.TagValue << "'";
				rst = stmt->executeQuery(var1.str());
				rst->first();
				std::stringstream comando;
				char *var2 = new char[2];
				memcpy(var2,Word1.c_str(),2);
				comando << "UPDATE city" << c.getCurrentCityID() << " SET Metadata='Building=" << building.TagValue << ";RemainingTime=" << rst->getInt("time") << ";MJStored=0;MJRemaining=" << rst->getInt("MJ") << ";' WHERE FieldX=" << util::lton(var2[0]) << " AND FieldY=" << var2[1];
				stmt->executeUpdate(comando.str());
				comando.str("");
				comando << "UPDATE city" << c.getCurrentCityID() << " SET Type='EDIFICIO' WHERE FieldX=" << util::lton(var2[0]) << " AND FieldY=" << var2[1];
				stmt->executeUpdate(comando.str());
				delete[] var2;
			} catch (sql::SQLException e)
			{
				std::cerr << e.what() << std::endl;
			}
			delete[] metadatos;
}