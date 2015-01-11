#include "Field.h"
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
#include <random>
string Field::ObtenerValor() 
{
	int CityID = this->CityID;
	sql::Driver *driver;
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *rst;
	stringstream command;
	driver=sql::mysql::get_mysql_driver_instance();
	conn=driver->connect("localhost","root","power500");
	conn->setSchema("wars");
	stmt=conn->createStatement();
	command << "SELECT * FROM City" << CityID << " WHERE FieldX=" << this->PosX << " AND FieldY=" << this->PosY;
	rst=stmt->executeQuery(command.str());
	if (rst->rowsCount() != 1) return "ERROR";
	rst->first();
	return rst->getString("Type");
}
int Field::Guardar() 
{
	int X = this->PosX;
	int Y = this->PosY;
	int CityID = this->CityID;
	sql::Driver *driver;
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *rst;
	stringstream command;
	driver=sql::mysql::get_mysql_driver_instance();
	conn=driver->connect("localhost","root","power500");
	conn->setSchema("wars");
	stmt=conn->createStatement();
	command << "SELECT * FROM City" << CityID << " WHERE FieldX=" << X << " AND FieldY=" << Y;
	rst=stmt->executeQuery(command.str());
	command.str("");
	if (rst->rowsCount() == 0)
	{
		command << "INSERT INTO City" << CityID << " VALUES (" << X << "," << Y << ",'" << this->Value << "','')";
		stmt->executeUpdate(command.str());
		command.str("");
		conn->close();
		return 0;
	}
	else
	{
		command << "UPDATE City" << CityID << " SET Type='" << this->Value << "' WHERE FieldX=" << X << " AND FieldY=" << Y;
		stmt->executeUpdate(command.str());
		conn->close();
		return 0;
	}
	/* En teoría, nunca se llega aquí... Error. */ return 1;
}
void Field::setX(int value)
{
	this->PosX=value;
}
void Field::setY(int value)
{
	this->PosY=value;
}
void Field::setCity(int value)
{
	this->CityID=value;
}
void Field::setValue(std::string value)
{
	this->Value=value;
}
void Field::clear()
{
	this->CityID=0;
	this->PosX=0;
	this->PosY=0;
	this->Value="";
}
void Ray::Extend(int City, int StartingX, int StartingY, int ExtendProb,int MaxCount, std::string ChangingValue, std::string Direction)
{
	this->CityID = City;
	this->CurrentX = StartingX;
	this->CurrentY = StartingY;
	this->Value = ChangingValue;
	this->Direction = Direction;
	this->ExtendProb = ExtendProb;
	this->MaxCount = MaxCount;
	std::tr1::random_device rd;
	std::tr1::mt19937 gen(rd());
	std::tr1::uniform_int<int> dis(1,100);
	int aleatorio=dis(gen);
	int Count=0;
	while (aleatorio < this->ExtendProb && Count < this->MaxCount)
	{
		if (this->Direction=="N")
		{
			this->CurrentY=this->CurrentY + 1;
		}
		if (this->Direction=="S")
		{
			this->CurrentY=this->CurrentY - 1;
		}
		if (this->Direction=="E")
		{
			this->CurrentX=this->CurrentX + 1;
		}
		if (this->Direction=="O")
		{
			this->CurrentX=this->CurrentX - 1;
		}
		if (this->Direction=="NE")
		{
			this->CurrentY=this->CurrentY + 1;
			this->CurrentX=this->CurrentX + 1;
		}
		if (this->Direction=="NO")
		{
			this->CurrentY=this->CurrentY + 1;
			this->CurrentX=this->CurrentX - 1;
		}
		if (this->Direction=="SE")
		{
			this->CurrentY=this->CurrentY - 1;
			this->CurrentX=this->CurrentX + 1;
		}
		if (this->Direction=="SO")
		{
			this->CurrentY=this->CurrentY - 1;
			this->CurrentX=this->CurrentX - 1;
		}
		if (this->CurrentX > 20 || this->CurrentX <= 0 || this->CurrentY > 10 || this->CurrentY <= 0) break;
		Field check;
		check.setCity(this->CityID);
		check.setX(this->CurrentX);
		check.setY(this->CurrentY);
		string valor = check.ObtenerValor();
		if (valor != "CAMPO" && valor != "PIEDRAS") break;
		Field nuevacasilla;
		nuevacasilla.setCity(this->CityID);
		nuevacasilla.setX(this->CurrentX);
		nuevacasilla.setY(this->CurrentY);
		nuevacasilla.setValue(this->Value);
		nuevacasilla.Guardar();
		aleatorio = dis(gen);
		++Count;
	}
}