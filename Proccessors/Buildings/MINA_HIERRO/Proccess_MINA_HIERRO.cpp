#include "Proccess_MINA_HIERRO.h"
#include "..\..\..\Common\SQL.h"
#include "..\..\..\Util\metadataseparator.h"
#include "..\..\..\Util\LettertoNumber.h"
#include <sstream>

void tasks::buildings::proccess_MINA_HIERRO(int CityID,std::string Field)
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
	int sacarCobre = atoi(size.TagValue.c_str()) * 7500;
	int energiaNecesaria = sacarCobre * 10;
	int RemainingCopper = atoi(Remaining.TagValue.c_str());
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
	CurrentMJint -= energiaNecesaria;
	int CurrentCopper = atoi(Stored.TagValue.c_str());
	CurrentCopper += sacarCobre;
	std::stringstream nuevo;
	nuevo << "Type=PERFORADORA_PETROLEO;Size=" << atoi(size.TagValue.c_str()) << ";Remaining=" << RemainingCopper - sacarCobre << ";MJStored=" << CurrentMJint << ";StoredIron=" << CurrentCopper << ";";
	std::stringstream comando;
	comando << "UPDATE city" << CityID << " SET Metadata='" << nuevo.str() << "' WHERE FieldX=" << FieldX << " AND FieldY=" << FieldY;
	stmt->executeUpdate(comando.str());
	std::cout << "[City Simulator] Mina de hierro procesada. " << sacarCobre << " unidades de hierro fueron extraidas, quedando " << RemainingCopper << " unidades de hierro en el yacimiento." << std::endl;
	delete[] fields;
	delete[] metadatac;
}