#include "CityCreation.h"
#include "Field.h"
#include <sstream>
#include <string>
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
#include <stdlib.h>
#include <random>
using namespace std;
tr1::random_device rd;
int CityCreation::CrearCiudad(std::string usuario)
{
	sql::Driver *driver;
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *rst;
	driver=sql::mysql::get_mysql_driver_instance();
	conn=driver->connect("localhost","root","power500");
	conn->setSchema("wars");
	stmt=conn->createStatement();
	stringstream command;
	int lastID;
	int userID;
	rst=stmt->executeQuery("SELECT cityID FROM cities ORDER BY cityID DESC");
	rst->first();
	if (rst->rowsCount()==0) {lastID=1;}
	else {lastID=rst->getInt("cityID")+1;}
	command.str("");
	command << "SELECT userID FROM users WHERE userName='" << usuario << "'";
	rst=stmt->executeQuery(command.str()); rst->first();
	command.str("");
	userID=rst->getInt("userID");
	command << "UPDATE users SET DefaultCity=" << lastID << " WHERE userID=" << userID;
	stmt->executeUpdate(command.str());
	command.str("");
	tr1::mt19937 gen(rd());
	tr1::uniform_int<int> dis(0,1000);
repetirXY:
	int XPos = dis(gen);
	int YPos = dis(gen);
	//Comprueba que no hay ninguna ciudad con las mismas coordenadas
	command << "SELECT * FROM cities WHERE PositionX=" << XPos << " AND PositionY=" << YPos;
	rst=stmt->executeQuery(command.str());
	command.str("");
	if (rst->rowsCount() != 0) goto repetirXY;
	command << "INSERT INTO cities VALUES (" << lastID << ",'" << usuario << "city'," << userID << ",'" << usuario << "',0," << XPos << "," << YPos << ",0)";
	stmt->executeUpdate(command.str());
	command.str("");
	command << "CREATE TABLE City" << lastID << " (FieldX int, FieldY int, Type varchar(30), Metadata varchar(255))";
	stmt->executeUpdate(command.str());
	//CREATE CITY
	tr1::uniform_int<int> dis2(1,10);
	int aleatnumber=dis2(gen);
	string predeterminado="CAMPO";
	if (aleatnumber == 1) predeterminado="PIEDRAS";
	Field casilla;
	casilla.setCity(lastID);
	int iResult;
	//Generate fields
	for (int X=1;X<=20;X++)
	{
		for (int Y=1;Y<=10;Y++)
		{
			casilla.setX(X);
			casilla.setY(Y);
			casilla.setValue(predeterminado);
			iResult=casilla.Guardar();
			if (iResult != 0) return 1;
		}
	}
	// Generate town hall
	casilla.clear();
	casilla.setCity(lastID);
	casilla.setX(10);
	casilla.setY(5);
	casilla.setValue("AYUNTAMIENTO");
	iResult=casilla.Guardar();
	if (iResult != 0) return 1;
	// Water generation
	// First chunk
	bool resultado;
	do
	{
		tr1::mt19937 gen3(rd());
		tr1::mt19937 gen4(rd());
		tr1::uniform_int<int> dis3(1,5);
		tr1::uniform_int<int> dis4(1,10);
		int randomnum = dis3(gen3);
		if (randomnum == 1)
		{
			int randomX = dis4(gen4);
			int randomY = dis3(gen3);
			resultado = CityCreation::CreateWater(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	// Second chunk
	do
	{
		tr1::mt19937 gen3(rd());
		tr1::mt19937 gen4(rd());
		tr1::uniform_int<int> dis3(1,5);
		tr1::uniform_int<int> XResize(11,20);
		int randomnum = dis3(gen3);
		if (randomnum == 1)
		{
			int randomX = XResize(gen4);
			int randomY = dis3(gen3);
			resultado = CityCreation::CreateWater(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Third chunk
	do
	{
		tr1::mt19937 gen3(rd());
		tr1::mt19937 gen4(rd());
		tr1::uniform_int<int> dis3(1,5);
		tr1::uniform_int<int> XResize(1,10);
		tr1::uniform_int<int> YResize(6,10);
		int randomnum = dis3(gen3);
		if (randomnum == 1)
		{
			int randomX = XResize(gen4);
			int randomY = YResize(gen3);
			resultado = CityCreation::CreateWater(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Fourth chunk
	do
	{
		tr1::mt19937 gen3(rd());
		tr1::mt19937 gen4(rd());
		tr1::uniform_int<int> dis3(1,5);
		tr1::uniform_int<int> XResize(11,20);
		tr1::uniform_int<int> YResize(6,10);
		int randomnum = dis3(gen3);
		if (randomnum == 1)
		{
			int randomX = XResize(gen4);
			int randomY = YResize(gen3);
			resultado = CityCreation::CreateWater(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Oil generation
	//First chunk
	do
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(1,10);
		tr1::uniform_int<int> YResize(1,5);
		int randomnum = RandomGen(Gen);
		if (randomnum == 1)
		{
			int randomX = XResize(Gen);
			int randomY = YResize(Gen);
			resultado = CityCreation::CreateOil(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Second chunk
	do
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(11,20);
		tr1::uniform_int<int> YResize(1,5);
		int randomnum = RandomGen(Gen);
		if (randomnum == 1)
		{
			int randomX = XResize(Gen);
			int randomY = YResize(Gen);
			resultado = CityCreation::CreateOil(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Third chunk
	do
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(1,10);
		tr1::uniform_int<int> YResize(6,10);
		int randomnum = RandomGen(Gen);
		if (randomnum == 1)
		{
			int randomX = XResize(Gen);
			int randomY = YResize(Gen);
			resultado = CityCreation::CreateOil(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Fourth chunk
	do
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(11,20);
		tr1::uniform_int<int> YResize(6,10);
		int randomnum = RandomGen(Gen);
		if (randomnum == 1)
		{
			int randomX = XResize(Gen);
			int randomY = YResize(Gen);
			resultado = CityCreation::CreateOil(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Gas generation
	//First chunk
	do
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(1,10);
		tr1::uniform_int<int> YResize(1,5);
		int randomnum = RandomGen(Gen);
		if (randomnum == 1)
		{
			int randomX = XResize(Gen);
			int randomY = YResize(Gen);
			resultado = CityCreation::CreateGas(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Second chunk
	do
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(11,20);
		tr1::uniform_int<int> YResize(1,5);
		int randomnum = RandomGen(Gen);
		if (randomnum == 1)
		{
			int randomX = XResize(Gen);
			int randomY = YResize(Gen);
			resultado = CityCreation::CreateGas(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Third chunk
	do
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(1,10);
		tr1::uniform_int<int> YResize(6,10);
		int randomnum = RandomGen(Gen);
		if (randomnum == 1)
		{
			int randomX = XResize(Gen);
			int randomY = YResize(Gen);
			resultado = CityCreation::CreateGas(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Fourth chunk
	do
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(11,20);
		tr1::uniform_int<int> YResize(6,10);
		int randomnum = RandomGen(Gen);
		if (randomnum == 1)
		{
			int randomX = XResize(Gen);
			int randomY = YResize(Gen);
			resultado = CityCreation::CreateGas(lastID,randomX,randomY);
		}
		else resultado = true;
	} while (resultado == false);
	//Copper generation
	for (int k=0;k<10;++k)
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> RandomVein(1,100);
		tr1::uniform_int<int> XResize(1,20);
		tr1::uniform_int<int> YResize(1,10);
		int randomnum=RandomGen(Gen);
		if (randomnum != 1) continue;
		int RandomX = XResize(Gen);
		int RandomY = YResize(Gen);
		Field nuevacasilla;
		nuevacasilla.setCity(lastID);
		nuevacasilla.setX(RandomX);
		nuevacasilla.setY(RandomY);
		string valor=nuevacasilla.ObtenerValor();
		if (valor != "CAMPO" && valor != "PIEDRAS") continue;
		nuevacasilla.setValue("COBRE");
		int VeinRandom=RandomVein(Gen);
		if (VeinRandom <= 40)
		{
			//Casilla solitaria
			nuevacasilla.Guardar();
		}
		if (VeinRandom > 40 && VeinRandom <= 65)
		{
			//1x2 vertical
			Field compruebacasilla;
			compruebacasilla.setCity(lastID);
			compruebacasilla.setX(RandomX);
			compruebacasilla.setY(RandomY + 1);
			string check = compruebacasilla.ObtenerValor();
			if (check != "CAMPO" && check != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("COBRE");
			compruebacasilla.Guardar();
		}
		if (VeinRandom > 65 && VeinRandom <= 90)
		{
			//2x1 horizontal
			Field compruebacasilla;
			compruebacasilla.setCity(lastID);
			compruebacasilla.setX(RandomX + 1);
			compruebacasilla.setY(RandomY);
			string check = compruebacasilla.ObtenerValor();
			if (check != "CAMPO" && check != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("COBRE");
			compruebacasilla.Guardar();
		}
		if (VeinRandom > 90)
		{
			//2x2
			Field compruebacasilla;
			Field compruebacasilla2;
			Field compruebacasilla3;
			compruebacasilla.setCity(lastID);
			compruebacasilla2.setCity(lastID);
			compruebacasilla3.setCity(lastID);
			compruebacasilla.setX(RandomX + 1);
			compruebacasilla.setY(RandomY);
			compruebacasilla2.setX(RandomX);
			compruebacasilla2.setY(RandomY + 1);
			compruebacasilla3.setX(RandomX + 1);
			compruebacasilla3.setY(RandomY + 1);
			string check1 = compruebacasilla.ObtenerValor();
			if (check1 != "CAMPO" && check1 != "PIEDRAS") continue;
			string check2 = compruebacasilla2.ObtenerValor();
			if (check2 != "CAMPO" && check2 != "PIEDRAS") continue;
			string check3 = compruebacasilla3.ObtenerValor();
			if (check3 != "CAMPO" && check3 != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("COBRE");
			compruebacasilla.Guardar();
			compruebacasilla2.setValue("COBRE");
			compruebacasilla2.Guardar();
			compruebacasilla3.setValue("COBRE");
			compruebacasilla3.Guardar();
		}

	}
	//Iron generation
	for (int k=0;k<10;++k)
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> RandomVein(1,100);
		tr1::uniform_int<int> XResize(1,20);
		tr1::uniform_int<int> YResize(1,10);
		int randomnum=RandomGen(Gen);
		if (randomnum != 1) continue;
		int RandomX = XResize(Gen);
		int RandomY = YResize(Gen);
		Field nuevacasilla;
		nuevacasilla.setCity(lastID);
		nuevacasilla.setX(RandomX);
		nuevacasilla.setY(RandomY);
		string valor=nuevacasilla.ObtenerValor();
		if (valor != "CAMPO" && valor != "PIEDRAS") continue;
		nuevacasilla.setValue("HIERRO");
		int VeinRandom=RandomVein(Gen);
		if (VeinRandom <= 40)
		{
			//Casilla solitaria
			nuevacasilla.Guardar();
		}
		if (VeinRandom > 40 && VeinRandom <= 65)
		{
			//1x2 vertical
			Field compruebacasilla;
			compruebacasilla.setCity(lastID);
			compruebacasilla.setX(RandomX);
			compruebacasilla.setY(RandomY + 1);
			string check = compruebacasilla.ObtenerValor();
			if (check != "CAMPO" && check != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("HIERRO");
			compruebacasilla.Guardar();
		}
		if (VeinRandom > 65 && VeinRandom <= 90)
		{
			//2x1 horizontal
			Field compruebacasilla;
			compruebacasilla.setCity(lastID);
			compruebacasilla.setX(RandomX + 1);
			compruebacasilla.setY(RandomY);
			string check = compruebacasilla.ObtenerValor();
			if (check != "CAMPO" && check != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("HIERRO");
			compruebacasilla.Guardar();
		}
		if (VeinRandom > 90)
		{
			//2x2
			Field compruebacasilla;
			Field compruebacasilla2;
			Field compruebacasilla3;
			compruebacasilla.setCity(lastID);
			compruebacasilla2.setCity(lastID);
			compruebacasilla3.setCity(lastID);
			compruebacasilla.setX(RandomX + 1);
			compruebacasilla.setY(RandomY);
			compruebacasilla2.setX(RandomX);
			compruebacasilla2.setY(RandomY + 1);
			compruebacasilla3.setX(RandomX + 1);
			compruebacasilla3.setY(RandomY + 1);
			string check1 = compruebacasilla.ObtenerValor();
			if (check1 != "CAMPO" && check1 != "PIEDRAS") continue;
			string check2 = compruebacasilla2.ObtenerValor();
			if (check2 != "CAMPO" && check2 != "PIEDRAS") continue;
			string check3 = compruebacasilla3.ObtenerValor();
			if (check3 != "CAMPO" && check3 != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("HIERRO");
			compruebacasilla.Guardar();
			compruebacasilla2.setValue("HIERRO");
			compruebacasilla2.Guardar();
			compruebacasilla3.setValue("HIERRO");
			compruebacasilla3.Guardar();
		}

	}
	//Coal generation
	for (int k=0;k<10;++k)
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> RandomVein(1,100);
		tr1::uniform_int<int> XResize(1,20);
		tr1::uniform_int<int> YResize(1,10);
		int randomnum=RandomGen(Gen);
		if (randomnum != 1) continue;
		int RandomX = XResize(Gen);
		int RandomY = YResize(Gen);
		Field nuevacasilla;
		nuevacasilla.setCity(lastID);
		nuevacasilla.setX(RandomX);
		nuevacasilla.setY(RandomY);
		string valor=nuevacasilla.ObtenerValor();
		if (valor != "CAMPO" && valor != "PIEDRAS") continue;
		nuevacasilla.setValue("COAL");
		int VeinRandom=RandomVein(Gen);
		if (VeinRandom <= 40)
		{
			//Casilla solitaria
			nuevacasilla.Guardar();
		}
		if (VeinRandom > 40 && VeinRandom <= 65)
		{
			//1x2 vertical
			Field compruebacasilla;
			compruebacasilla.setCity(lastID);
			compruebacasilla.setX(RandomX);
			compruebacasilla.setY(RandomY + 1);
			string check = compruebacasilla.ObtenerValor();
			if (check != "CAMPO" && check != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("COAL");
			compruebacasilla.Guardar();
		}
		if (VeinRandom > 65 && VeinRandom <= 90)
		{
			//2x1 horizontal
			Field compruebacasilla;
			compruebacasilla.setCity(lastID);
			compruebacasilla.setX(RandomX + 1);
			compruebacasilla.setY(RandomY);
			string check = compruebacasilla.ObtenerValor();
			if (check != "CAMPO" && check != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("COAL");
			compruebacasilla.Guardar();
		}
		if (VeinRandom > 90)
		{
			//2x2
			Field compruebacasilla;
			Field compruebacasilla2;
			Field compruebacasilla3;
			compruebacasilla.setCity(lastID);
			compruebacasilla2.setCity(lastID);
			compruebacasilla3.setCity(lastID);
			compruebacasilla.setX(RandomX + 1);
			compruebacasilla.setY(RandomY);
			compruebacasilla2.setX(RandomX);
			compruebacasilla2.setY(RandomY + 1);
			compruebacasilla3.setX(RandomX + 1);
			compruebacasilla3.setY(RandomY + 1);
			string check1 = compruebacasilla.ObtenerValor();
			if (check1 != "CAMPO" && check1 != "PIEDRAS") continue;
			string check2 = compruebacasilla2.ObtenerValor();
			if (check2 != "CAMPO" && check2 != "PIEDRAS") continue;
			string check3 = compruebacasilla3.ObtenerValor();
			if (check3 != "CAMPO" && check3 != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("COAL");
			compruebacasilla.Guardar();
			compruebacasilla2.setValue("COAL");
			compruebacasilla2.Guardar();
			compruebacasilla3.setValue("COAL");
			compruebacasilla3.Guardar();
		}
	}
	//Uranium generation
	for (int k=0;k<10;++k)
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> RandomVein(1,100);
		tr1::uniform_int<int> XResize(1,20);
		tr1::uniform_int<int> YResize(1,10);
		int randomnum=RandomGen(Gen);
		if (randomnum != 1) continue;
		int RandomX = XResize(Gen);
		int RandomY = YResize(Gen);
		Field nuevacasilla;
		nuevacasilla.setCity(lastID);
		nuevacasilla.setX(RandomX);
		nuevacasilla.setY(RandomY);
		string valor=nuevacasilla.ObtenerValor();
		if (valor != "CAMPO" && valor != "PIEDRAS") continue;
		nuevacasilla.setValue("URANIO");
		int VeinRandom=RandomVein(Gen);
		if (VeinRandom <= 40)
		{
			//Casilla solitaria
			nuevacasilla.Guardar();
		}
		if (VeinRandom > 40 && VeinRandom <= 65)
		{
			//1x2 vertical
			Field compruebacasilla;
			compruebacasilla.setCity(lastID);
			compruebacasilla.setX(RandomX);
			compruebacasilla.setY(RandomY + 1);
			string check = compruebacasilla.ObtenerValor();
			if (check != "CAMPO" && check != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("URANIO");
			compruebacasilla.Guardar();
		}
		if (VeinRandom > 65 && VeinRandom <= 90)
		{
			//2x1 horizontal
			Field compruebacasilla;
			compruebacasilla.setCity(lastID);
			compruebacasilla.setX(RandomX + 1);
			compruebacasilla.setY(RandomY);
			string check = compruebacasilla.ObtenerValor();
			if (check != "CAMPO" && check != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("URANIO");
			compruebacasilla.Guardar();
		}
		if (VeinRandom > 90)
		{
			//2x2
			Field compruebacasilla;
			Field compruebacasilla2;
			Field compruebacasilla3;
			compruebacasilla.setCity(lastID);
			compruebacasilla2.setCity(lastID);
			compruebacasilla3.setCity(lastID);
			compruebacasilla.setX(RandomX + 1);
			compruebacasilla.setY(RandomY);
			compruebacasilla2.setX(RandomX);
			compruebacasilla2.setY(RandomY + 1);
			compruebacasilla3.setX(RandomX + 1);
			compruebacasilla3.setY(RandomY + 1);
			string check1 = compruebacasilla.ObtenerValor();
			if (check1 != "CAMPO" && check1 != "PIEDRAS") continue;
			string check2 = compruebacasilla2.ObtenerValor();
			if (check2 != "CAMPO" && check2 != "PIEDRAS") continue;
			string check3 = compruebacasilla3.ObtenerValor();
			if (check3 != "CAMPO" && check3 != "PIEDRAS") continue;
			nuevacasilla.Guardar();
			compruebacasilla.setValue("URANIO");
			compruebacasilla.Guardar();
			compruebacasilla2.setValue("URANIO");
			compruebacasilla2.Guardar();
			compruebacasilla3.setValue("URANIO");
			compruebacasilla3.Guardar();
		}
	}
	//Lava generation
	for (int k=0;k<10;++k)
	{
		tr1::mt19937 Gen(rd());
		tr1::uniform_int<int> RandomGen(1,10);
		tr1::uniform_int<int> XResize(1,20);
		tr1::uniform_int<int> YResize(1,10);
		int randomnum=RandomGen(Gen);
		if (randomnum != 1) continue;
		int RandomX = XResize(Gen);
		int RandomY = YResize(Gen);
		Field nuevacasilla;
		nuevacasilla.setCity(lastID);
		nuevacasilla.setX(RandomX);
		nuevacasilla.setY(RandomY);
		string valor=nuevacasilla.ObtenerValor();
		if (valor != "CAMPO" && valor != "PIEDRAS") continue;
		nuevacasilla.setValue("LAVA");
		//Casilla solitaria
		nuevacasilla.Guardar();
	}
	return 0;
}

bool CityCreation::CreateWater(int ID,int randomX,int randomY)
{
	Field cambiacasilla;
	cambiacasilla.setCity(ID);
	cambiacasilla.setX(randomX);
	cambiacasilla.setY(randomY);
	string valor=cambiacasilla.ObtenerValor();
	if (valor=="AYUNTAMIENTO") return false;
	cambiacasilla.setValue("AGUA");
	cambiacasilla.Guardar();
	Ray rayoNorte;
	rayoNorte.Extend(ID,randomX,randomY,75,4,"AGUA","N");
	Ray rayoSur;
	rayoSur.Extend(ID,randomX,randomY,75,4,"AGUA","S");
	Ray rayoEste;
	rayoEste.Extend(ID,randomX,randomY,75,4,"AGUA","E");
	Ray rayoOeste;
	rayoOeste.Extend(ID,randomX,randomY,75,4,"AGUA","O");
	Ray rayoNoreste;
	rayoNoreste.Extend(ID,randomX,randomY,75,4,"AGUA","NE");
	Ray rayoNoroeste;
	rayoNoroeste.Extend(ID,randomX,randomY,75,4,"AGUA","NO");
	Ray rayoSureste;
	rayoSureste.Extend(ID,randomX,randomY,75,4,"AGUA","SE");
	Ray rayoSuroeste;
	rayoSuroeste.Extend(ID,randomX,randomY,75,4,"AGUA","SO");
	return true;
}
bool CityCreation::CreateOil(int ID,int randomX,int randomY)
{
	Field cambiacasilla;
	cambiacasilla.setCity(ID);
	cambiacasilla.setX(randomX);
	cambiacasilla.setY(randomY);
	string valor=cambiacasilla.ObtenerValor();
	if (valor=="AYUNTAMIENTO" || valor=="AGUA") return false;
	cambiacasilla.setValue("OIL");
	cambiacasilla.Guardar();
	Ray rayoNorte;
	rayoNorte.Extend(ID,randomX,randomY,50,2,"OIL","N");
	Ray rayoSur;
	rayoSur.Extend(ID,randomX,randomY,50,2,"OIL","S");
	Ray rayoEste;
	rayoEste.Extend(ID,randomX,randomY,50,2,"OIL","E");
	Ray rayoOeste;
	rayoOeste.Extend(ID,randomX,randomY,50,2,"OIL","O");
	Ray rayoNoreste;
	rayoNoreste.Extend(ID,randomX,randomY,50,2,"OIL","NE");
	Ray rayoNoroeste;
	rayoNoroeste.Extend(ID,randomX,randomY,50,2,"OIL","NO");
	Ray rayoSureste;
	rayoSureste.Extend(ID,randomX,randomY,50,2,"OIL","SE");
	Ray rayoSuroeste;
	rayoSuroeste.Extend(ID,randomX,randomY,50,2,"OIL","SO");
	return true;
}
bool CityCreation::CreateGas(int ID,int randomX,int randomY)
{
	Field cambiacasilla;
	cambiacasilla.setCity(ID);
	cambiacasilla.setX(randomX);
	cambiacasilla.setY(randomY);
	string valor=cambiacasilla.ObtenerValor();
	if (valor=="AYUNTAMIENTO" || valor=="AGUA" ||valor=="OIL") return false;
	cambiacasilla.setValue("GAS");
	cambiacasilla.Guardar();
	Ray rayoNorte;
	rayoNorte.Extend(ID,randomX,randomY,50,2,"GAS","N");
	Ray rayoSur;
	rayoSur.Extend(ID,randomX,randomY,50,2,"GAS","S");
	Ray rayoEste;
	rayoEste.Extend(ID,randomX,randomY,50,2,"GAS","E");
	Ray rayoOeste;
	rayoOeste.Extend(ID,randomX,randomY,50,2,"GAS","O");
	Ray rayoNoreste;
	rayoNoreste.Extend(ID,randomX,randomY,50,2,"GAS","NE");
	Ray rayoNoroeste;
	rayoNoroeste.Extend(ID,randomX,randomY,50,2,"GAS","NO");
	Ray rayoSureste;
	rayoSureste.Extend(ID,randomX,randomY,50,2,"GAS","SE");
	Ray rayoSuroeste;
	rayoSuroeste.Extend(ID,randomX,randomY,50,2,"GAS","SO");
	return true;
}