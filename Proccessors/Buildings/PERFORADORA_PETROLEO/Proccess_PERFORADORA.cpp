#include "Proccess_PERFORADORA.h"
#include <cppconn\prepared_statement.h>
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
#include "..\..\..\Util\metadataseparator.h"
#include "..\..\..\Util\LettertoNumber.h"
#include <sstream>

void tasks::buildings::proccess_PERFORADORA_PETROLEO(int CityID,std::string Field)
{
	sql::Driver *driver;
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *rst;
	driver = sql::mysql::get_mysql_driver_instance();
	conn = driver->connect("localhost","root","power500");
	conn->setSchema("wars");
	stmt = conn->createStatement();
	char *fields = new char[Field.size()];
	memcpy(fields,Field.c_str(),Field.size());
	int FieldX = util::lton(fields[0]);
	int FieldY = atoi(&fields[1]);
	std::stringstream var1;
	var1 << "SELECT Metadata FROM city" << CityID << " WHERE FieldX=" << FieldX << " AND FieldY=" << FieldY;
	rst = stmt->executeQuery(var1.str());
	rst->first();
	std::string metadata = rst->getString("Metadata");
	char *metadatac = new char[metadata.size()];
	memcpy(metadatac,metadata.c_str(),metadata.size());
	Tag size = util::SeparateTags(metadatac,1);
	Tag Stored = util::SeparateTags(metadatac,4);
	Tag Remaining = util::SeparateTags(metadatac,2);
	int sacarOil = atoi(size.TagValue.c_str()) * 100;
	int energiaNecesaria = sacarOil * 150;
	int RemainingWater = atoi(Remaining.TagValue.c_str());
	Tag currentMJ = util::SeparateTags(metadatac,3);
	if (atoi(currentMJ.TagValue.c_str()) < energiaNecesaria)
	{
		//Not enough energy to perform operation
		// TODO: (Far future) Add notification for user
		delete[] fields;
		delete[] metadatac;
		return;
	}
	int CurrentMJint = atoi(currentMJ.TagValue.c_str());
	CurrentMJint = CurrentMJint - energiaNecesaria;
	int CurrentOil = atoi(Stored.TagValue.c_str());
	CurrentOil-= sacarOil;
	std::stringstream nuevo;
	nuevo << "Type=PERFORADORA_PETROLEO;Size=" << atoi(size.TagValue.c_str()) << ";Remaining=" << RemainingWater - sacarOil << ";MJStored=" << CurrentMJint << ";StoredOil=" << CurrentOil << ";";
	std::stringstream comando;
	comando << "UPDATE city" << CityID << " SET Metadata='" << nuevo.str() << "' WHERE FieldX=" << FieldX << " AND FieldY=" << FieldY;
	stmt->executeUpdate(comando.str());
	std::cout << "[City Simulator] Perforadora de petroleo procesada. " << sacarOil << " unidades de petroleo fueron extraidas, quedando " << RemainingWater << " unidades de petroleo en el yacimiento." << std::endl;
	delete[] fields;
	delete[] metadatac;
}