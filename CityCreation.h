#pragma once
#include <string>
#include <stdlib.h>
namespace CityCreation
{
	int CrearCiudad(std::string usuario);
	bool CreateWater(int ID,int RandomX,int RandomY);
	bool CreateOil(int ID,int RandomX,int RandomY);
	bool CreateGas(int ID,int RandomX,int RandomY);
};