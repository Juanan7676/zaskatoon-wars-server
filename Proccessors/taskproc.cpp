#include "taskproc.h"
#include "..\Common\SQL.h"
#include <sstream>
#include "..\Util\metadataseparator.h"
#include "BUILD.h"
#include "Buildings\PERFORADORA_PETROLEO\Proccess_PERFORADORA.h"
#include "Buildings\BOMBA_DE_AGUA\Proccess_BOMBA.h"
#include "Buildings\MINA_COBRE\Proccess_MINA_COBRE.h"
#include "Buildings\MINA_HIERRO\Proccess_MINA_HIERRO.h"
#include "Buildings\CENTRAL_HIDROELECTRICA\CENTRAL_HIDROELECTRICA.h"

Task::Task(int TaskID) 
{
	try
	{
		sql::Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *rst;
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("localhost","root","power500"); conn->setSchema("wars");
		conn->setSchema("wars");
		stmt = conn->createStatement();
		std::stringstream comando;
		comando << "SELECT * FROM tasks WHERE TaskID=" << TaskID;
		rst = stmt->executeQuery(comando.str()); rst->first();
		if (rst->rowsCount() == 0)
		{
			std::stringstream var1;
			var1 << "Task ID no. " << TaskID << " was not found.";
			throw new InvalidTaskIDException(var1.str());
		}
		this->TaskID = rst->getInt("TaskID");
		this->metadata = rst->getString("Metadata");
	} catch (InvalidTaskIDException e)
	{
		std::cerr << e.what();
		this->TaskID = NULL;
		this->metadata = "";
	}
	catch (sql::SQLException e)
	{
		std::cerr << e.what();
		this->TaskID = NULL;
		this->metadata = "";
	}
}

InvalidTaskIDException::InvalidTaskIDException(std::string message)
{
	this->message = message;
}

int Task::proccess()
{
	int TaskID = this->TaskID;
	sql::Driver *d;
	sql::Connection *c;
	sql::Statement *s;
	sql::ResultSet *rst;
	d = sql::mysql::get_mysql_driver_instance();
	c = d->connect("localhost","root","power500");
	c->setSchema("wars");
	s = c->createStatement();
	std::stringstream command;
	command << "SELECT * FROM tasks WHERE TaskID=" << TaskID;
	rst = s->executeQuery(command.str()); rst->first();
	std::string metadatos = rst->getString("Metadata");
	char *Metadata = new char[metadatos.size() + 1];
	memcpy(Metadata,metadatos.c_str(),metadatos.size());
	Tag tag = util::SeparateTags(Metadata, 0);
	if (tag.TagName == "Type")
	{
		if (tag.TagValue == "Build")
		{
			Tag city = util::SeparateTags(Metadata, 1);
			Tag field = util::SeparateTags(Metadata, 2);
			tasks::ProccessBuild(atoi(city.TagValue.c_str()),field.TagValue,TaskID);
		}
		if (tag.TagValue == "Proccess")
		{
			Tag city = util::SeparateTags(Metadata, 2);
			Tag field = util::SeparateTags(Metadata, 3); 
			char *cfield = new char[2]; 
			memcpy(cfield,field.TagValue.c_str(),field.TagValue.size());
			std::stringstream X;
			X << cfield[0];
			std::stringstream Y;
			Y << cfield[1];
			Tag building = util::SeparateTags(Metadata, 1);
			Building b;
			b.City = atoi(city.TagValue.c_str());
			b.metadata = Metadata;
			b.PosX = atoi(X.str().c_str());
			b.PosY = atoi(Y.str().c_str());
			if (building.TagValue == "BOMBA_DE_AGUA")
			{
				tasks::buildings::proccess_BOMBA_DE_AGUA(atoi(city.TagValue.c_str()),field.TagValue);
			}
			if (building.TagValue == "PERFORADORA_PETROLEO")
			{
				tasks::buildings::proccess_PERFORADORA_PETROLEO(atoi(city.TagValue.c_str()),field.TagValue);
			}
			if (building.TagValue == "MINA_COBRE")
			{
				tasks::buildings::proccess_MINA_COBRE(atoi(city.TagValue.c_str()),field.TagValue);
			}
			if (building.TagValue == "MINA_HIERRO")
			{
				tasks::buildings::proccess_MINA_HIERRO(atoi(city.TagValue.c_str()),field.TagValue);
			}
			if (building.TagValue == "CENTRAL_HIDROELECTRICA")
			{
				tasks::buildings::proccess_central_hidroelectrica(b);
			}
			// TODO: More buildings
			delete[] cfield;
		}
		// TODO: More tasks
	}
	delete[] Metadata;
	return 0;
}

Task::Task(std::string metadata)
{
	this->metadata = metadata;
}

void Task::push(int priority)
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
		rst = stmt->executeQuery("SELECT * FROM tasks ORDER BY TaskID DESC");
		int ID;
		if (rst->rowsCount() == 0) ID = 1;
		else
		{
			rst->first();
			ID = rst->getInt("TaskID") + 1;
		}
		std::stringstream comando;
		comando << "INSERT INTO tasks VALUES (" << ID << "," << priority << "'" << this->metadata << "')";
		stmt->executeUpdate(comando.str());
	} catch (sql::SQLException e)
	{
		std::cerr << "SQL Exception ocurred in push(): " << e.what() << std::endl;
	}
}