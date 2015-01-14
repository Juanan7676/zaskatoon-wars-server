#include "taskproc.h"
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
	return 0;
}