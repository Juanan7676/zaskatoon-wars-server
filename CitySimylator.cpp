#include "CitySimylator.h"
#include <time.h>
#include "windows.h"
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
#include "Proccessors\taskproc.h"

unsigned __stdcall CitySimulator::StartThread(void* data)
{
	time_t stamp = time(NULL);
	struct tm tiempo;
	errno_t err = localtime_s(&tiempo,&stamp);
	while(1) //Must add a "stop simulation" and command-line in main thread. Pointer to a bool variable (data) and when it is true, exit loop.
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
		rst = stmt->executeQuery("SELECT * FROM tasks");
		rst->first();
		if (rst->next())
		{
			rst->first();
			do
			{
				Task task = Task(rst->getInt("TaskID"));
				task.proccess();
			} while (rst->next());
		}
		int minuto;
		do
		{
			Sleep(1000);
			minuto = tiempo.tm_min;
		} while (minuto != 0); //Check for next update
	}
	return 0;
}
