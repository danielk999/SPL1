#include "Customer.h"
#include <iostream>
using namespace std;

Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){}

Customer::Customer(Customer & other):name(other.getName()),id(other.getId()){}

Customer::Customer(Customer && other):name(other.getName()),id(other.getId()){}

Customer::~Customer(){}

std::string Customer::getName() const
{
	return name;
}

int Customer::getId() const
{
	return id;
}

VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id){}

Customer * VegetarianCustomer::clone()
{
	Customer *toReturn = new VegetarianCustomer(*this);
	return toReturn;
}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish>& menu)
{
	int smallestid(-1), price(0), idofbvg = 0;;
	for (int i = 0;i<(int) menu.size(); i++)
	{
		if((smallestid==-1) & (menu.at(i).getType()==0))
		{
			smallestid = menu.at(i).getId();
		}
		else if ((menu.at(i).getType() == 0) & (smallestid > menu.at(i).getId())) 
		{
			smallestid = menu.at(i).getId();
		}
		else if ((menu.at(i).getType() == 2) & (price < menu.at(i).getPrice()))
		{
			price = menu.at(i).getPrice();
			idofbvg= menu.at(i).getId();
		}
		else if ((menu.at(i).getType() == 2) & (price == menu.at(i).getPrice()) & (idofbvg > menu.at(i).getId()))
		{
			idofbvg = menu.at(i).getId();
		}
	}
	if ((smallestid == -1) & (price == 0))
	{
		return std::vector<int>();
	}
	else if (smallestid == -1 )
	{
		std::vector<int> toReturn;
		toReturn.push_back(idofbvg);
		return toReturn;
	}
	else if (price == 0)
	{
		std::vector<int> toReturn;
		toReturn.push_back(smallestid);
		return toReturn;
	}
	else
	{
		std::vector<int> toReturn;
		toReturn.push_back(smallestid);
		toReturn.push_back(idofbvg);
		return toReturn;
	}
}

std::string VegetarianCustomer::toString() const
{
	std::string toReturn;
	toReturn += getName();
	toReturn += ',';
	toReturn += "veg";
	return toReturn;
}

CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){}

Customer * CheapCustomer::clone()
{
	Customer *toReturn = new CheapCustomer(*this);
	return toReturn;
}

std::vector<int> CheapCustomer::order(const std::vector<Dish>& menu)
{
	if(!ordered){
		int lowestprice = menu.at(0).getPrice(), lowestid = 0;
		for (int i = 1; i < (int)menu.size(); i++)
		{
			if (lowestprice > menu.at(i).getPrice())
			{
				lowestprice = menu.at(i).getPrice();
				lowestid = menu.at(i).getId();
			}
			else if ((lowestprice == menu.at(i).getPrice()) & (lowestid > menu.at(i).getId()))
			{
				lowestid = menu.at(i).getId();
			}
		}
		ordered=true;
		std::vector<int> toReturn;
		toReturn.push_back(lowestid);
		return toReturn;
	}
	return std::vector<int>();
}

std::string CheapCustomer::toString() const
{
	std::string toReturn;
	toReturn += getName();
	toReturn += ',';
	toReturn += "chp";
	return toReturn;
}

SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id)
{
}

Customer * SpicyCustomer::clone()
{
	Customer *toReturn = new SpicyCustomer(*this);
	return toReturn;
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish>& menu)
{
	if (!hasOrdered) 
	{
		int mostExp = -1, id = 0;
		for (int i = 0; i < (int)menu.size(); i++)
		{
			if ((mostExp==-1) & (menu.at(i).getType()==1))
			{
				mostExp = menu.at(i).getPrice();
				id = menu.at(i).getId();
			}
			else if ((mostExp < menu.at(i).getPrice()) & (menu.at(i).getType() == 1))
			{
				mostExp = menu.at(i).getPrice();
			}
			else if ((mostExp == menu.at(i).getPrice()) & (menu.at(i).getType() == 2) & (id>menu.at(i).getId()))
			{
				id = menu.at(i).getId();
			}
		}
		hasOrdered = true;
		if (mostExp == -1)
		{
			return std::vector<int>();
		}
		else 
		{
			std::vector<int> toReturn;
			toReturn.push_back(id);
			return toReturn;
		}
	}
	else
	{
		int leastexp = -1, id = 0;
		for (int i = 0; i < (int)menu.size(); i++)
		{
			if ((leastexp == -1) & (menu.at(i).getType() == 2))
			{
				leastexp = menu.at(i).getPrice();
				id = menu.at(i).getId();
			}
			else if ((leastexp > menu.at(i).getPrice()) & (menu.at(i).getType() == 2))
			{
				leastexp = menu.at(i).getPrice();
				id = menu.at(i).getId();
			}
			else if ((leastexp == menu.at(i).getPrice()) & (menu.at(i).getType() == 2) & (id>menu.at(i).getId()))
			{
				id = menu.at(i).getId();
			}
		}
		if (leastexp == -1)
		{
			return std::vector<int>();
		}
		else
		{
			std::vector<int> toReturn;
			toReturn.push_back(id);
			return toReturn;
		}
	}
}

std::string SpicyCustomer::toString() const
{
	std::string toReturn="";
	toReturn += getName();
	toReturn += ',';
	toReturn += "spc";
	return toReturn;
}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id): Customer(name, id)
{
}

Customer * AlchoholicCustomer::clone()
{
	Customer *toReturn = new AlchoholicCustomer(*this);
	return toReturn;
}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish>& menu)
{
	int price = -1, id = 0;
	for (int i = 0; i < (int)menu.size(); i++)
	{
		if ((price == -1) & (menu.at(i).getType() == 3) & (lastPrice < menu.at(i).getPrice()))
		{
			price = menu.at(i).getPrice();
			id = menu.at(i).getId();
		}
		else if ((price == -1) & (menu.at(i).getType() == 3) & (lastPrice == menu.at(i).getPrice()) & (lastId < menu.at(i).getId()))
		{
			price = menu.at(i).getPrice();
			id = menu.at(i).getId();
		}
		else if ((price > menu.at(i).getPrice()) & (menu.at(i).getType() == 3) & (lastPrice < menu.at(i).getPrice()))
		{
			price = menu.at(i).getPrice();
			id = menu.at(i).getId();
		}
		else if ((price == menu.at(i).getPrice()) & (menu.at(i).getType() == 3) & (lastPrice < menu.at(i).getPrice()) & (id > menu.at(i).getId()))
		{
			id = menu.at(i).getId();
		}
	}
	if (price != -1)
	{
		lastPrice = price;
		lastId = id;
		std::vector<int> toReturn;
		toReturn.push_back(id);
		return toReturn;
	}
	return std::vector<int>();
}

std::string AlchoholicCustomer::toString() const
{
	std::string toReturn="";
	toReturn += getName();
	toReturn += ',';
	toReturn += "alc";
	return toReturn;
}
