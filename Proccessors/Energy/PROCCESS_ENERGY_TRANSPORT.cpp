#include "PROCCESS_ENERGY_TRANSPORT.h"
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
#include <vector>
#include "..\..\Util\metadataseparator.h"
#include "..\..\Util\ChangeMetadataValue.h"
#include <sstream>
#include <string>

void tasks::ProccessEnergy(Building origen, Building destino)
{
	try
	{
		// Initialize stuff

		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("localhost","root","power500");
		conn->setSchema("wars");
		stmt = conn->createStatement();

		//Get buildings metadatas

		std::stringstream comando;
		comando << "SELECT * FROM city" << origen.City << " WHERE FieldX=" << origen.PosX << " AND FieldY=" << origen.PosY;
		rst = stmt->executeQuery(comando.str());
		rst->first();
		std::string mOrigen = rst->getString("Metadata");
		char *cOrigen = new char[255];
		memcpy(cOrigen,mOrigen.c_str(),mOrigen.size());
		cOrigen[mOrigen.size() + 1] = '\0';
		comando.str("");
		comando << "SELECT * FROM city" << destino.City << " WHERE FieldX=" << destino.PosX << " AND FieldY=" << origen.PosY;
		rst = stmt->executeQuery(comando.str());
		rst->first();
		std::string mDestino = rst->getString("Metadata");
		char *cDestino = new char[255];
		memcpy(cDestino,mDestino.c_str(),mDestino.size());
		cDestino[mDestino.size() + 1] = '\0';
		comando.str("");
		TagList lOrigen;

		//Get MJ Amount of origin building and empty it

		int mark = 0;
		for (int k=0;lOrigen.at(k).TagName != "ERROR";++k)
		{
			lOrigen.push_back(util::SeparateTags(cOrigen,k));
			if (lOrigen.at(k).TagName == "MJStored") mark = k;
		}
		int MJToMove = atoi(lOrigen.at(mark).TagValue.c_str());
		lOrigen.at(mark).TagValue = "0";
		util::changemetadata(cOrigen,lOrigen);
		comando << "UPDATE city" << origen.City << " SET Metadata='" << cOrigen << "' WHERE FieldX=" << origen.PosX << " AND FieldY=" << origen.PosY;
		stmt->executeUpdate(comando.str());

		//Add MJ to destination building, but...

		int loss = 10;
		if (origen.City == origen.City) loss = 0;
		int MJToAdd = MJToMove - (MJToMove * loss / 100);
		mark = 0;
		TagList lDestino;
		for (int k=0;lDestino.at(k).TagName != "ERROR";++k)
		{
			lDestino.push_back(util::SeparateTags(cDestino,k));
			if (lDestino.at(k).TagName == "MJStored") mark = k;
		}
		lDestino.at(mark).TagValue = MJToAdd;
		util::changemetadata(cDestino,lDestino);
		comando.str("");
		comando << "UPDATE city" << destino.City << " SET Metadata='" << mDestino << "' WHERE FieldX=" << destino.PosX << " AND FieldY=" << destino.PosY;
		stmt->executeUpdate(comando.str());
		delete[] cOrigen;
		delete[] cDestino;
	}
	catch (sql::SQLException e)
	{
		std::cerr << "SQL Exception in ProccessEnergy(): " << e.what();
	}
	catch (std::out_of_range e)
	{
		std::cerr << "Out of Range exception in ProccessEnergy(): " << e.what();
	}
}

void tasks::ProccessEnergy(Building origen, Building destino, int MJAmount)
{
	try
	{
		// Initialize stuff

		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("localhost","root","power500");
		conn->setSchema("wars");
		stmt = conn->createStatement();

		//Get buildings metadatas

		std::stringstream comando;
		comando << "SELECT * FROM city" << origen.City << " WHERE FieldX=" << origen.PosX << " AND FieldY=" << origen.PosY;
		rst = stmt->executeQuery(comando.str());
		rst->first();
		std::string mOrigen = rst->getString("Metadata");
		char *cOrigen = new char[255];
		memcpy(cOrigen,mOrigen.c_str(),mOrigen.size());
		cOrigen[mOrigen.size() + 1] = '\0';
		comando.str("");
		comando << "SELECT * FROM city" << destino.City << " WHERE FieldX=" << destino.PosX << " AND FieldY=" << origen.PosY;
		rst = stmt->executeQuery(comando.str());
		rst->first();
		std::string mDestino = rst->getString("Metadata");
		char *cDestino = new char[255];
		memcpy(cDestino,mDestino.c_str(),mDestino.size());
		cDestino[mDestino.size() + 1] = '\0';
		comando.str("");
		TagList lOrigen;

		//Get MJ Amount of origin building and empty the amount of MJ removed

		int mark = 0;
		for (int k=0;lOrigen.at(k).TagName != "ERROR";++k)
		{
			lOrigen.push_back(util::SeparateTags(cOrigen,k));
			if (lOrigen.at(k).TagName == "MJStored") mark = k;
		}
		int MJToMove = MJAmount;
		lOrigen.at(mark).TagValue = atoi(lOrigen.at(mark).TagValue.c_str()) - MJToMove;
		util::changemetadata(cOrigen,lOrigen);
		comando << "UPDATE city" << origen.City << " SET Metadata='" << cOrigen << "' WHERE FieldX=" << origen.PosX << " AND FieldY=" << origen.PosY;
		stmt->executeUpdate(comando.str());

		//Add MJ to destination building, but...

		int loss = 10;
		if (origen.City == origen.City) loss = 0;
		int MJToAdd = MJToMove - (MJToMove * loss / 100);
		mark = 0;
		TagList lDestino;
		for (int k=0;lDestino.at(k).TagName != "ERROR";++k)
		{
			lDestino.push_back(util::SeparateTags(cDestino,k));
			if (lDestino.at(k).TagName == "MJStored") mark = k;
		}
		lDestino.at(mark).TagValue = MJToAdd;
		util::changemetadata(cDestino,lDestino);
		comando.str("");
		comando << "UPDATE city" << destino.City << " SET Metadata='" << mDestino << "' WHERE FieldX=" << destino.PosX << " AND FieldY=" << destino.PosY;
		stmt->executeUpdate(comando.str());
		delete[] cOrigen;
		delete[] cDestino;
	}
	catch (sql::SQLException e)
	{
		std::cerr << "SQL Exception in ProccessEnergy(): " << e.what();
	}
	catch (std::out_of_range e)
	{
		std::cerr << "Out of Range exception in ProccessEnergy(): " << e.what();
	}
}