#include "Dish.h"
using namespace std;

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type)
	: id(d_id), name(d_name), price(d_price), type(d_type)
{
}

//Dish::Dish(const Dish & other)
//	: id(other.id), name(other.name), price(other.price), type(other.type)
//{
//}

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

//Dish::~Dish()
//{
//}
