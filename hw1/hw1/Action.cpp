#include "Action.h"
#include "Table.h"
#include "Restaurant.h"

using namespace std;

BaseAction::BaseAction(): status(PENDING), errorMsg()
{
}

ActionStatus BaseAction::getStatus() const
{
	return status;
}

void BaseAction::complete()
{
	status = COMPLETED;
}

void BaseAction::error(std::string errorMsg)
{
	status = ERROR;
	this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const
{
	return errorMsg;
}

string BaseAction::statusToString() const
{
	if (getStatus() == COMPLETED)
		return "Completed";
	if (getStatus() == PENDING)
		return "Pending";
	if (getStatus() == ERROR)
		return "Error: " + getErrorMsg();
	return "";
}

OpenTable::OpenTable(int id, std::vector<Customer*>& customersList): tableId(id), customers(customersList)
{
}

void OpenTable::act(Restaurant & restaurant)
{
	Table* t = restaurant.getTable(tableId);
	if (t == nullptr || t->isOpen() || customers.size() > t->getCapacity())
	{
		error("Table does not exist or is already open");
		return;
	}
	t->openTable();
	customersStr = "";
	for (int i = 0; i < customers.size(); i++)
	{
		t->addCustomer(customers.at(i));
		customersStr += customers.at(i)->toString();
		customersStr += " ";
	}
	customers.clear();
	complete();
}

std::string OpenTable::toString() const
{
	return "open " + to_string(tableId) + ' ' + customersStr + statusToString();
}

BaseAction * OpenTable::clone()
{
	return new OpenTable(tableId, customers);
}

Order::Order(int id): tableId(id)
{
}

void Order::act(Restaurant & restaurant)
{
	Table* t = restaurant.getTable(tableId);
	if (t == nullptr || !t->isOpen())
	{
		error("Table does not exist or is not open");
		return;
	}
	vector<OrderPair> oldOrders = t->getOrders();
	t->order(restaurant.getMenu());
	vector<OrderPair> newOrders = t->getOrders();
	for (int i = 0; i < oldOrders.size(); i++)
	{
		for (int j = 0; j < newOrders.size(); j++)
		{
			if (oldOrders.at(i) == newOrders.at(j))
			{
				newOrders.at(j).first = -1;
				oldOrders.at(i).first = -1;
				break;
			}
		}
	}
	for (int i = 0; i < newOrders.size(); i++)
	{
		if(newOrders.at(i).first != -1)
			cout << t->getCustomer(newOrders.at(i).first)->getName() << " ordered " << newOrders.at(i).second.getName() << endl;
	}
	complete();
}

std::string Order::toString() const
{
	return "order " + to_string(tableId) + ' ' + statusToString();
}

BaseAction * Order::clone()
{
	return new Order(tableId);
}

Close::Close(int id):tableId(id)
{
}

void Close::act(Restaurant & restaurant)
{
	Table* t= restaurant.getTable(tableId);
	if(t==nullptr || !t->isOpen)
	{
		this->error("Table does not exist or is not open");
	}
	else
	{
		int price = 0;
		std::vector<OrderPair>& orders = t->getOrders();
		for (int i = 0; i < orders.size(); i++)
		{
			price += orders.at(i).second.getPrice();
		}
		std::cout << "Table " + to_string(tableId) + " was closed. Bill " + to_string(price) + "NIS" << std::endl;
		t->closeTable();
		this->complete();
	}
}

std::string Close::toString() const
{
	return "close " + to_string(tableId) + " " + this->statusToString();
}

BaseAction * Close::clone()
{
	return new Close(tableId);
}

CloseAll::CloseAll()
{
}

void CloseAll::act(Restaurant & restaurant)
{
	for (int i = 0; i < restaurant.getNumOfTables; i++) {
		if (restaurant.getTable(i)->isOpen()) {
			Close c(i + 1);
			c.act(restaurant);
		}
	}
	this->complete();
}

std::string CloseAll::toString() const
{
	return "closeall "+ this->statusToString();
}

BaseAction * CloseAll::clone()
{
	return new CloseAll();
}

PrintTableStatus::PrintTableStatus(int id):tableId(id)
{
}

void PrintTableStatus::act(Restaurant & restaurant)
{
	if (tableId <= restaurant.getNumOfTables) {
		Table *t = restaurant.getTable(tableId);
		cout << "Table " + to_string(tableId) + " status " + to_string(t->isOpen) << endl;
		if (t->isOpen) {
			std::vector<Customer*> customers = t->getCustomers();
			for (int i = 0; i < customers.size(); i++) {
				cout << to_string(customers.at(i)->getId()) + " " + customers.at(i)->getName() << endl;
			}
			std::vector<OrderPair> orders = t->getOrders();
			for (int i = 0; i < orders.size(); i++) {
				cout << orders.at(i).second.getName() + " " + to_string(orders.at(i).second.getPrice)+"NIS"+" "+to_string(orders.at(i).first) << endl;
			}
		}
	}
	this->complete();
}

std::string PrintTableStatus::toString() const
{
	return "status " + to_string(tableId) + " " + to_string(this->getStatus());
}

BaseAction * PrintTableStatus::clone()
{
	return new PrintTableStatus(tableId);
}
