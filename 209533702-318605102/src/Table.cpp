#include "Table.h"

using namespace std;

Table::Table(int t_capacity): capacity(t_capacity), open(false), customersList(), orderList()
{
}


Table::Table(const Table & other): capacity(other.getCapacity()), open(other.open), customersList(), orderList(other.orderList)
{
	for (int i = 0; i < (int)other.customersList.size(); i++)
		customersList.push_back(other.customersList.at(i)->clone());
}

Table::Table(Table && other): capacity(other.getCapacity()), open(other.open), customersList(), orderList(other.orderList)
{
	for (int i = 0; i < (int)other.customersList.size(); i++)
	{
		customersList.push_back(other.customersList.at(i));
	}
	other.orderList.clear();
	other.customersList.clear();
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

std::vector<OrderPair> Table::getCustomerOrders(int id)
{
	vector<OrderPair> orders;
	for (int i = 0; i < (int)orderList.size(); i++)
		if (orderList.at(i).first == id)
			orders.push_back(orderList.at(i));
	return orders;
}

void Table::removeCustomerOrders(int id)
{
	vector<OrderPair> orders;
	for (int i = 0; i < (int)orderList.size(); i++)
		if (orderList.at(i).first != id)
			orders.push_back(orderList.at(i));	
	for (int i = 0; i < (int)orders.size(); i++)
	{
		orderList.push_back(OrderPair(orders.at(i).first,Dish(orders.at(i).second.getId(),orders.at(i).second.getName(),orders.at(i).second.getPrice(),orders.at(i).second.getType())));
	}
}

void Table::addOrders(std::vector<OrderPair> orders)
{
	for (int i = 0; i < (int)orders.size(); i++)
		orderList.push_back(orders.at(i));
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

Table::~Table()
{
	for (vector<Customer*>::iterator it = customersList.begin(); it != customersList.end(); ++it)
		delete *it;
	customersList.clear();
}

Table & Table::operator=(const Table & other)
{
	if (this == &other) {
		return *this;
	}
	for (vector<Customer*>::iterator it = customersList.begin(); it != customersList.end(); ++it)
		delete *it;
	customersList.clear();
	orderList.clear();
	capacity = other.capacity;
	open = other.open;
	for (int i = 0; i < (int)other.customersList.size(); i++)
		customersList.push_back(other.customersList.at(i)->clone());
	for (int i = 0; i < (int)other.orderList.size(); i++)
		orderList.push_back(other.orderList.at(i));
	return *this;
}

Table & Table::operator=(Table && other)
{
	if (this != &other)
	{
		for (vector<Customer*>::iterator it = customersList.begin(); it != customersList.end(); ++it)
			delete *it;
		customersList.clear();
		orderList.clear();
		capacity = other.capacity;
		open = other.open;
		for (int i = 0; i < (int)other.customersList.size(); i++)
		{
			customersList.push_back(other.customersList.at(i));
		}
		for (int i = 0; i < (int)other.orderList.size(); i++)
		{
			orderList.push_back(other.orderList.at(i));
		}
		other.orderList.clear();
		other.customersList.clear();
	}
	return *this;
}

Dish Table::findDish(int id, const vector<Dish>& menu)
{
	for (int i = 0; i < (int)menu.size(); i++)
		if (menu.at(i).getId() == id)
			return menu.at(i);
	return Dish(-1, "", 0, BVG);
}
