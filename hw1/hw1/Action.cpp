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

MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTable(src), dstTable(dst), id(customerId)
{
}

void MoveCustomer::act(Restaurant & restaurant)
{
	Table* src = restaurant.getTable(srcTable);
	Table* dst = restaurant.getTable(dstTable);
	Customer* c;
	if (src != nullptr)
		c = src->getCustomer(id);
	if (src == nullptr || !src->isOpen() || dst == nullptr || !dst->isOpen() || c == nullptr || dst->getCapacity == dst->getCustomers().size())
	{
		error("Cannot move customer");
		return;
	}
	vector<OrderPair> orders = src->getCustomerOrders(id);
	src->removeCustomerOrders(id);
	src->removeCustomer(id);	
	if (src->getCustomers().size() == 0)
		src->closeTable();
	dst->addCustomer(c);
	dst->addOrders(orders);
	complete();
}

std::string MoveCustomer::toString() const
{
	return "move " + to_string(srcTable) + ' ' + to_string(dstTable) + ' ' + to_string(id);
}

BaseAction * MoveCustomer::clone()
{
	return new MoveCustomer(srcTable, dstTable, id);
}

PrintMenu::PrintMenu()
{
}

void PrintMenu::act(Restaurant & restaurant)
{
	vector<Dish> menu = restaurant.getMenu();
	for (int i = 0; i < menu.size(); i++)
		cout << menu.at(i).toString() << endl;
}

std::string PrintMenu::toString() const
{
	return "menu "+statusToString();
}

BaseAction * PrintMenu::clone()
{
	return new PrintMenu();
}
