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
#include <iostream>
#include <sstream>

unsigned __stdcall CitySimulator::StartThread(void* data)
{
	bool *simulate = (bool *)data;
	try
	{
		time_t stamp = time(NULL);
		struct tm tiempo;
		errno_t err = localtime_s(&tiempo,&stamp);
		while(1)
		{
			std::cout << "[City Simulator] Simulation Started." << std::endl;
			sql::Driver *driver;
			sql::Connection *conn;
			sql::Statement *stmt;
			sql::ResultSet *rst;
			driver = sql::mysql::get_mysql_driver_instance();
			conn = driver->connect("localhost","root","power500");
			conn->setSchema("wars");
			stmt = conn->createStatement();
			std::stringstream comando;
			for (int priority=0;priority <= 3; priority++)
			{
				std::stringstream comando;
				comando << "SELECT * FROM tasks WHERE PRIORITY=" << priority;
				rst = stmt->executeQuery(comando.str());
				comando.str("");
				rst->first();
				if (rst->rowsCount() != 0)
				{
					rst->first();
					do
					{
						Task task = Task(rst->getInt("TaskID"));
						task.proccess();
					} while (rst->next());
				}
			}
			int minuto;
			do
			{
				Sleep(1000);
				minuto = tiempo.tm_min;
				if (*simulate == true)
				{
					*simulate = false;
					break;
				}
			} while (minuto != 0); //Check for next update
		}
	}
	catch (sql::SQLException e)
	{
		std::cerr << "Se ha producido una excepcion SQL: " << e.what() << std::endl;
		return 0;
	}
	catch (std::out_of_range out)
	{
		std::cerr << "Excepcion: " << out.what() << std::endl;
	}
	catch (std::exception e)
	{
		std::cerr << "Se ha producido una excepcion en CitySimylator.cpp: " << e.what() << std::endl;
		return 0;
	}
	return 0;
}
