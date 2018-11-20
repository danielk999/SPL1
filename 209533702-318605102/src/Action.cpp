#include "Action.h"
#include "Table.h"
#include "Restaurant.h"

using namespace std;

BaseAction::BaseAction(): errorMsg(),status(PENDING)
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

BaseAction::~BaseAction(){}

void BaseAction::error(std::string errorMsg)
{
	cout<<"Error: " + errorMsg<<endl;
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

OpenTable::OpenTable(int id, std::vector<Customer*>& customersList): tableId(id), customers(customersList),customersStr("")
{
}

void OpenTable::act(Restaurant & restaurant)
{
	customersStr = "";
	for (int i = 0; i < (int)customers.size(); i++)
	{
		customersStr += customers.at(i)->toString();
		customersStr += " ";
	}
	Table* t = restaurant.getTable(tableId);
	if ((t == nullptr) || (t->isOpen()) || ((int)customers.size() > t->getCapacity()))
	{
		error("Table does not exist or is already open");
		return;
	}
	t->openTable();
	for (int i = 0; i < (int)customers.size(); i++)
	{
		t->addCustomer(customers.at(i));
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
	return new OpenTable(tableId, customersStr, getStatus(), getErrorMsg());
}

OpenTable::OpenTable(int id, std::string customersStr, ActionStatus status, std::string errorMsg): this.id(id), this.customersStr(customersStr), this.status(status), this.errorMsg(errorMsg)
{
}

Order::Order(int id): tableId(id)
{
}

void Order::act(Restaurant & restaurant)
{
	Table* t = restaurant.getTable(tableId);
	if ((t == nullptr) || (!t->isOpen()))
	{
		error("Table does not exist or is not open");
		return;
	}
	vector<OrderPair> oldOrders = t->getOrders();
	t->order(restaurant.getMenu());
	vector<OrderPair> newOrders = t->getOrders();
	for (int i = 0; i < (int)oldOrders.size(); i++)
	{
		for (int j = 0; j < (int)newOrders.size(); j++)
		{
			if ((oldOrders.at(i).first == newOrders.at(j).first)&(oldOrders.at(i).second.getId() == newOrders.at(j).second.getId()))
			{
				newOrders.at(j).first = -1;
				oldOrders.at(i).first = -1;
				break;
			}
		}
	}
	for (int i = 0; i < (int)newOrders.size(); i++)
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
	return new Order(tableId,getStatus(), getErrorMsg());
}

Order::Order(int id, ActionStatus status, std::string errorMsg): id(tableId), this.status(status), this.errorMsg(errorMsg)
{
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
	if ((src == nullptr) || (!src->isOpen()) || (dst == nullptr) || (!dst->isOpen()) || (c == nullptr) || (dst->getCapacity() == (int)dst->getCustomers().size()))
	{
		error("Cannot move customer");
		return;
	}
	vector<OrderPair> orders = src->getCustomerOrders(id);
	src->removeCustomerOrders(id);
	src->removeCustomer(id);	
	if ((int)src->getCustomers().size() == 0)
		src->closeTable();
	dst->addCustomer(c);
	dst->addOrders(orders);
	complete();
}

std::string MoveCustomer::toString() const
{
	return "move " + to_string(srcTable) + ' ' + to_string(dstTable) + ' ' + to_string(id)+" "+statusToString();
}

BaseAction * MoveCustomer::clone()
{
	return new MoveCustomer(srcTable, dstTable, id, getStatus(), getErrorMsg());
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId, ActionStatus status, std::string errorMsg)
	:srcTable(src), dstTable(dst), id(customerId), this.status(status), this.errorMsg(errorMsg)
{
}

Close::Close(int id):tableId(id)
{
}

void Close::act(Restaurant & restaurant)
{
	Table* t= restaurant.getTable(tableId);
	if((t==nullptr) || (!t->isOpen()))
	{
		this->error("Table does not exist or is not open");
	}
	else
	{
		int price = 0;
		std::vector<OrderPair>& orders = t->getOrders();
		for (int i = 0; i < (int)orders.size(); i++)
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
	return new Close(tableId, getStatus(), getErrorMsg());
}

Close::Close(int id, ActionStatus status, std::string errorMsg)
	: this.id(id), this.status(status), this.errorMsg(errorMsg)
{
}

CloseAll::CloseAll()
{
}

void CloseAll::act(Restaurant & restaurant)
{
	for (int i = 0; i < restaurant.getNumOfTables(); i++) {
		if (restaurant.getTable(i)->isOpen()) {
			Close c(i);
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
	return new CloseAll(getStatus(), getErrorMsg());
}

CloseAll::CloseAll(ActionStatus status, std::string errorMsg): this.status(status), this.errorMsg(errorMsg)
{
}

PrintMenu::PrintMenu()
{
}

void PrintMenu::act(Restaurant & restaurant)
{
	vector<Dish> menu = restaurant.getMenu();
	for (int i = 0; i < (int)menu.size(); i++)
		cout << menu.at(i).toString() << endl;
	complete();
}

std::string PrintMenu::toString() const
{
	return "menu "+statusToString();
}

BaseAction * PrintMenu::clone()
{
	return new PrintMenu(getStatus(), getErrorMsg());
}

PrintMenu::PrintMenu(ActionStatus status, std::string errorMsg) : this.status(status), this.errorMsg(errorMsg)
{
}

PrintTableStatus::PrintTableStatus(int id):tableId(id)
{
}

void PrintTableStatus::act(Restaurant & restaurant)
{
	if (tableId <= restaurant.getNumOfTables()) {
		Table *t = restaurant.getTable(tableId);
		std::string s;
		if(t->isOpen()){
			s="open";
		}
		else{
			s="close";
		}
		cout << "Table " + to_string(tableId) + " status: " + s << endl;
		if (t->isOpen()) {
			std::vector<Customer*> customers = t->getCustomers();
			cout << "Customers:"<< endl;
			for (int i = 0; i < (int)customers.size(); i++) {
				cout << to_string(customers.at(i)->getId()) + " " + customers.at(i)->getName() << endl;
			}
			std::vector<OrderPair> orders = t->getOrders();
			cout << "Orders:" << endl;
			for (int i = 0; i < (int)orders.size(); i++) {
				cout << orders.at(i).second.getName() + " " + to_string(orders.at(i).second.getPrice())+"NIS"+" "+to_string(orders.at(i).first) << endl;
			}
			cout << "Current Bill: " + to_string(t->getBill())+"NIS" << endl;
		}
	}
	this->complete();
}

std::string PrintTableStatus::toString() const
{
	return "status " + to_string(tableId) + " " + statusToString();
}

BaseAction * PrintTableStatus::clone()
{
	return new PrintTableStatus(tableId, getStatus(), getErrorMsg());
}

PrintTableStatus::PrintTableStatus(int id, ActionStatus status, std::string errorMsg) : this.id(id), this.status(status), this.errorMsg(errorMsg)
{
}

PrintActionsLog::PrintActionsLog()
{
}

void PrintActionsLog::act(Restaurant & restaurant)
{
	vector<BaseAction*> actions = restaurant.getActionsLog();
	for (int i = 0; i <(int)actions.size(); i++)
		cout << actions.at(i)->toString() << endl;
	complete();
}

std::string PrintActionsLog::toString() const
{
	return "log "+statusToString();
}

BaseAction * PrintActionsLog::clone()
{
	return new PrintActionsLog(getStatus(), getErrorMsg());
}

PrintActionsLog::PrintActionsLog(ActionStatus status, std::string errorMsg) : this.status(status), this.errorMsg(errorMsg)
{
}

BackupRestaurant::BackupRestaurant()
{
}

void BackupRestaurant::act(Restaurant & restaurant)
{
	if(backup==nullptr)
		backup=new Restaurant(restaurant);
	else
	{
		*backup = restaurant;
	}
	complete();
}

std::string BackupRestaurant::toString() const
{
	return "backup "+statusToString();
}

BaseAction * BackupRestaurant::clone()
{
	return new BackupRestaurant(getStatus(), getErrorMsg());
}

BackupRestaurant::BackupRestaurant(ActionStatus status, std::string errorMsg) : this.status(status), this.errorMsg(errorMsg)
{
}

RestoreResturant::RestoreResturant()
{
}

void RestoreResturant::act(Restaurant & restaurant)
{
	if (backup == nullptr)
	{
		error("No backup available");
		return;
	}
	restaurant = *backup;
	complete();
}

std::string RestoreResturant::toString() const
{
	return "restore "+statusToString();
}

BaseAction * RestoreResturant::clone()
{
	return new RestoreResturant(getStatus(), getErrorMsg());
}

RestoreResturant::RestoreResturant(ActionStatus status, std::string errorMsg) : this.status(status), this.errorMsg(errorMsg)
{
}
