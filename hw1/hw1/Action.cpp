#include "Action.h"
#include "Table.h"

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

OpenTable::OpenTable(int id, std::vector<Customer*>& customersList): tableId(id), customers(customersList)
{
}

void OpenTable::act(Restaurant & restaurant)
{
	Table* t = restaurant.getTable(tableId);
	if (t == nullptr | t->isOpen())
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
	string statusStr;
	if (getStatus() == COMPLETED)
		statusStr = "Completed";
	if (getStatus() == PENDING)
		statusStr = "Pending";
	if (getStatus() == ERROR)
		statusStr = "Error: " + getErrorMsg();
	return "open " + to_string(tableId) + ' ' + customersStr + statusStr;
}

BaseAction * OpenTable::clone()
{
	return new OpenTable(tableId, customers);
}
