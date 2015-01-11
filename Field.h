#pragma once
#include <string>
#include <iostream>
using namespace std;
class Field
{
private:
	int PosY;
	int PosX;
	int CityID;
	string Value;
public:
	int Guardar();
	string ObtenerValor();
	void setY(int value);
	void setX(int value);
	void setCity(int value);
	void setValue(string value);
	void clear();
};
class Ray
{
private:
	int CurrentX;
	int CurrentY;
	int CityID;
	int ExtendProb;
	int MaxCount;
	string Value;
	string Direction;
public:
	void Extend(int City, int StartingX, int StartingY, int ExtendProb,int MaxCount, string ChangingValue, string Direction);
};