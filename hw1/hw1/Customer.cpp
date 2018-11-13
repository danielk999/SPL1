#include "Customer.h"

Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){}

Customer::Customer(Customer & other):id(other.getId()),name(other.getName()){}

Customer::Customer(Customer && other):id(other.getId()),name(other.getName()){}

Customer::~Customer(){}

Customer & Customer::operator=(const Customer & other)
{
	if (this == &other) {
		return *this;
	}
	return *this;
}

Customer & Customer::operator=(Customer && other)
{
	if (this == &other) {
		return *this;
	}
	return *this;
}

std::string Customer::getName() const
{
	return name;
}

int Customer::getId() const
{
	return id;
}

VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id){}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish>& menu)
{
	int smallestid(-1), price(0), idofbvg = 0;;
	for (int i = 0; menu.size(); i++)
	{
		if(smallestid==-1 & menu.at(i).getType()==0)
		{
			smallestid = menu.at(i).getId();
		}
		else if (menu.at(i).getType() == 0 & smallestid > menu.at(i).getId()) 
		{
			smallestid = menu.at(i).getId();
		}
		else if (menu.at(i).getType() == 2 & price < menu.at(i).getPrice())
		{
			price = menu.at(i).getPrice();
			idofbvg= menu.at(i).getId();
		}
		else if (menu.at(i).getType() == 2 & price == menu.at(i).getPrice() & idofbvg > menu.at(i).getId())
		{
			idofbvg = menu.at(i).getId();
		}
	}
	if (smallestid == -1 & price == 0)
	{
		return std::vector<int>();
	}
	else if (smallestid == -1 )
	{
		return std::vector<int>(idofbvg);
	}
	else if (price == 0)
	{
		return std::vector<int>(smallestid);
	}
	else
	{
		return std::vector<int>(smallestid, idofbvg);
	}
}

std::string VegetarianCustomer::toString() const
{
	std::string toReturn;
	toReturn += this->getId();
	toReturn += getName();
	return toReturn;
}

CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){}

std::vector<int> CheapCustomer::order(const std::vector<Dish>& menu)
{
	int lowestprice = menu.at(0).getPrice(), lowestid = 0;
	for (int i = 1; i < menu.size(); i++)
	{
		if (lowestprice > menu.at(i).getPrice())
		{
			lowestprice = menu.at(i).getPrice();
			lowestid = menu.at(i).getId();
		}
		else if (lowestprice == menu.at(i).getPrice() & lowestid > menu.at(i).getId())
		{
			lowestid = menu.at(i).getId();
		}
	}
	return std::vector<int>(lowestid);
}

std::string CheapCustomer::toString() const
{
	std::string toReturn;
	toReturn += this->getId();
	toReturn += getName();
	return toReturn;
}

SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id)
{
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish>& menu)
{
	if (!hasOrdered) 
	{
		int mostExp = -1, id = 0;
		for (int i = 0; i < menu.size(); i++)
		{
			if (mostExp==-1 & menu.at(i).getType()==1)
			{
				mostExp = menu.at(i).getPrice();
				id = menu.at(i).getId();
			}
			else if (mostExp < menu.at(i).getPrice() & menu.at(i).getType() == 1)
			{
				mostExp = menu.at(i).getPrice();
			}
			else if (mostExp == menu.at(i).getPrice() & menu.at(i).getType() == 2 & id>menu.at(i).getId())
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
		return std::vector<int>(id);
		}
	}
	else
	{
		int leastexp = -1, id = 0;
		for (int i = 0; i < menu.size(); i++)
		{
			if (leastexp == -1 & menu.at(i).getType() == 2)
			{
				leastexp = menu.at(i).getPrice();
				id = menu.at(i).getId();
			}
			else if (leastexp > menu.at(i).getPrice() & menu.at(i).getType() == 2)
			{
				leastexp = menu.at(i).getPrice();
				id = menu.at(i).getId();
			}
			else if (leastexp == menu.at(i).getPrice() & menu.at(i).getType() == 2 & id>menu.at(i).getId())
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
			return std::vector<int>(id);
		}
	}
}

std::string SpicyCustomer::toString() const
{
	std::string toReturn;
	toReturn += this->getId();
	toReturn += getName();
	return toReturn;
}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id): Customer(name, id)
{
}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish>& menu)
{
	int price = 0, id = 0;
	for (int i = 0; i < menu.size(); i++)
	{
		if (price == 0 & menu.at(i).getType() == 3 & lastPrice == menu.at(i).getPrice() & lastId < menu.at(i).getId())
		{
			price = menu.at(i).getPrice();
			id = menu.at(i).getId();
		}
		else if (price == 0 & menu.at(i).getType() == 3 & lastPrice > menu.at(i).getPrice())
		{
			price = menu.at(i).getPrice();
			id = menu.at(i).getId();
		}
		else if (price > menu.at(i).getPrice() & menu.at(i).getType() == 3 & lastPrice > menu.at(i).getPrice())
		{
			price = menu.at(i).getPrice();
			id = menu.at(i).getId();
		}
		else if (price == menu.at(i).getPrice() & menu.at(i).getType() == 3 & lastPrice > menu.at(i).getPrice() & id > menu.at(i).getId())
		{
			id = menu.at(i).getId();
		}
	}
	if (price != 0)
	{
		lastPrice = price;
		lastId = id;
		return std::vector<int>(id);
	}
	return std::vector<int>(0);
}

std::string AlchoholicCustomer::toString() const
{
	std::string toReturn;
	toReturn += this->getId();
	toReturn += getName();
	return toReturn;
}
