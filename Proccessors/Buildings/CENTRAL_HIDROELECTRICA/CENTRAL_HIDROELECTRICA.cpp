#include "CENTRAL_HIDROELECTRICA.h"
#include "..\..\..\Common\SQL.h"
#include <sstream>
#include "..\..\..\Util\metadataseparator.h"
#include "..\..\..\Util\ChangeMetadataValue.h"

void tasks::buildings::proccess_central_hidroelectrica(Building b)
{
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
		std::stringstream comando;

		// Get building metadata

		comando << "SELECT * FROM city" << b.City << " WHERE FieldX=" << b.PosX << " AND FieldY=" << b.PosY;
		rst = stmt->executeQuery(comando.str());
		rst->first();
		char *metadata = new char[255];
		memcpy(metadata,rst->getString("Metadata").c_str(),rst->getString("Metadata").size());
		metadata[rst->getString("Metadata").size() + 1] = '\0';
		int markMJ,markWater,markUp1,markUp2=0;
		TagList t;
		for (int k=0;t.at(k).TagValue != "#ERROR#"; ++k)
		{
			t.push_back(util::SeparateTags(metadata,k));
			if (t.at(k).TagName == "MJStored") markMJ = k;
			if (t.at(k).TagName == "StoredWater") markWater = k;
			if (t.at(k).TagName == "EffUpgrades") markUp1 = k;
			if (t.at(k).TagName == "CapUpgrades") markUp2 = k;
		}
		
		// Calculate some stuff and push

		int WaterToConsume = 100 + (atoi(t.at(markUp2).TagValue.c_str()) * 100);
		if (WaterToConsume > atoi(t.at(markWater).TagValue.c_str())) WaterToConsume = atoi(t.at(markWater).TagValue.c_str()); //Future: Notification for user
		int ProducedMJ = WaterToConsume * (50 + (4 * atoi(t.at(markUp1).TagValue.c_str())));
		t.at(markWater).TagValue = atoi(t.at(markWater).TagValue.c_str()) - WaterToConsume;
		t.at(markMJ).TagValue = atoi(t.at(markMJ).TagValue.c_str()) + ProducedMJ;
		util::changemetadata(metadata,t);
		comando.str("");
		comando << "UPDATE city" << b.City << " SET Metadata='" << metadata << "' WHERE FieldX=" << b.PosX << " AND FieldY=" << b.PosY;
		stmt->executeUpdate(comando.str());
		std::cout << "[City Simulator] Central hidroelectrica procesada. Se produjeron " << ProducedMJ << " MJ a costa de " << WaterToConsume << " unidades de agua." << std::endl;
		delete[] metadata;
	}
	catch (std::out_of_range e)
	{
		std::cerr << "Exception occurred in CENTRAL_HIDROELECTRICA.cpp: " << e.what();
	}
}