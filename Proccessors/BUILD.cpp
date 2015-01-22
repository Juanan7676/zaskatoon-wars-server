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

void tasks::ProccessBuild(int cityID,std::string field)
{
	sql::Driver *driver;
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *rst;
	driver = sql::mysql::get_mysql_driver_instance();
	conn = driver->connect("localhost","root","power500");
	conn->setSchema("wars");
	std::stringstream comando;
	char *var1 = new char[2];
	memcpy(var1,field.c_str(),field.size());
	comando << "SELECT * FROM city" << cityID << " WHERE FieldX=" << var1[0] << " AND FieldY=" << var1[1];
	rst = stmt->executeQuery(comando.str()); rst->first();
	
}
