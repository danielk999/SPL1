#include "Dish.h"
using namespace std;

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type)
	: id(d_id), name(d_name), price(d_price), type(d_type)
{
}

int Dish::getId() const
{
	return id;
}

string Dish::getName() const
{
	return name;
}

int Dish::getPrice() const
{
	return price;
}

DishType Dish::getType() const
{
	return type;
}

std::string Dish::toString() const
{
	string strType;
	if (type == VEG)	strType = "VEG";
	if (type == SPC)	strType = "SPC";
	if (type == BVG)	strType = "BVG";
	if (type == ALC)	strType = "ALC";
	return name + ' ' + strType + ' ' + to_string(price) + "NIS";
}
