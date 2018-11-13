#include "Table.h"

using namespace std;

Table::Table(int t_capacity): capacity(t_capacity), open(false), customersList(), orderList()
{
}

int Table::getCapacity() const
{
	return capacity;
}

void Table::addCustomer(Customer * customer)
{
	customersList.push_back(customer);
}

void Table::removeCustomer(int id)
{
	for (vector<Customer*>::iterator it = customersList.begin(); it != customersList.end(); ++it)
		if ((*it)->getId() == id)
		{
			delete *it;
			customersList.erase(it);
			break;
		}
}

Customer * Table::getCustomer(int id)
{
	for (vector<Customer*>::iterator it = customersList.begin(); it != customersList.end(); ++it)
		if ((*it)->getId() == id)
			return *it;
	return nullptr;
}

vector<Customer*>& Table::getCustomers()
{
	return customersList;
}

std::vector<OrderPair>& Table::getOrders()
{
	return orderList;
}

void Table::order(const vector<Dish>& menu)
{
	for (vector<Customer*>::iterator it = customersList.begin(); it != customersList.end(); ++it)
	{
		vector<int> dishes =(*it)->order(menu);
		for (vector<int>::iterator it2 = dishes.begin(); it2 != dishes.end(); ++it2)
		{
			Dish d = findDish(*it2, menu);
			if (d.getId() != -1)
				orderList.push_back(OrderPair((*it)->getId(), d));
		}
	}
}

void Table::openTable()
{
	open = true;
}

void Table::closeTable()
{
	orderList.clear();
	for (vector<Customer*>::iterator it = customersList.begin(); it != customersList.end(); ++it)
		delete *it;
	customersList.clear();
	open = false;
}

int Table::getBill()
{
	int bill = 0;
	for (vector<OrderPair>::iterator it = orderList.begin(); it != orderList.end(); ++it)
	{
		bill += (*it).second.getPrice();
	}
	return bill;
}

bool Table::isOpen()
{
	return open;
}

Dish Table::findDish(int id, const vector<Dish>& menu)
{
	for (int i = 0; i < menu.size(); i++)
		if (menu.at(i).getId() == id)
			return menu.at(i);
	return Dish(-1, "", 0, BVG);
}
