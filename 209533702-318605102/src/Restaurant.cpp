#include "Restaurant.h"     
#include <sstream>
#include <fstream>  
using namespace std;    


Restaurant::Restaurant():open(false),tables(),menu(),actionsLog(){}

Restaurant::Restaurant(const std::string & configFilePath):open(false), tables(), menu(), actionsLog()
{
	std::ifstream file;
	file.open(configFilePath);
	std::vector<std::string> results;
	std::string input;
	while (std::getline(file, input, '\n')) {
		results.push_back(input);
	}
	int count = 1;
	for (int i = 0; i < (int)results.size(); i++)
	{
		
		if ((results.at(i)[0] != '#') & (results.at(i)!=""))
		{
			if (count == 1)
			{
				int length = stoi(results.at(i));
				count++;
				Tnumber = length;
			}
			else if(count==2)
			{
				std::vector<std::string> TSeats;
				std::istringstream iss(results.at(i));
				while (std::getline(iss, input, ',')) {
					TSeats.push_back(input);
				}
				for (int j = 0; (j < (int)TSeats.size()) & (j < Tnumber); j++)
				{
					int length = stoi(TSeats.at(j));
					tables.push_back(new Table(length));
				}
				count++;
			}
			else
			{
				std::istringstream iss(results.at(i));
				std::vector<std::string> dishes;
				while (std::getline(iss, input, ',')) {
					dishes.push_back(input);
				}
				DishType t;
				if (dishes.at(1) == "VEG")
				{
					t = VEG;
				}
				else if (dishes.at(1) == "SPC")
				{
					t = SPC;
				}
				else if (dishes.at(1) == "ALC")
				{
					t = ALC;
				}
				else
				{
					t = BVG;
				}
				menu.push_back(Dish(Did,dishes.at(0),stoi(dishes.at(2)),t));
				Did++;
			}
		}
	}
}

Restaurant::Restaurant(Restaurant & other):Tnumber(other.getNumOfTables()),open(other.isOpen()),tables(), menu(other.menu),actionsLog()
{
	for (int i = 0; i < (int)other.tables.size(); i++)
	{
		tables.push_back(new Table(*other.tables.at(i)));
	}
	for (int i = 0; i < (int)other.actionsLog.size(); i++)
	{
		actionsLog.push_back(other.actionsLog.at(i)->clone());
	}
}

Restaurant::Restaurant(Restaurant && other) : Tnumber(other.getNumOfTables()), open(other.isOpen()), tables(other.tables), menu(other.menu), actionsLog(other.actionsLog)
{
	other.tables.clear();
	other.actionsLog.clear();
}

Restaurant & Restaurant::operator=(Restaurant & other)
{
	if (this == &other)
		return *this;
	for (int i = 0; i < (int)tables.size(); i++) {
		delete tables.at(i);
	}
	for (int i = 0; i < (int)actionsLog.size(); i++) {
		delete actionsLog.at(i);
	}
	menu.clear();
	Tnumber = other.getNumOfTables();
	open = other.isOpen();
	for (int i = 0; i < (int)other.tables.size(); i++)
	{
		tables.push_back(new Table(*other.tables.at(i)));
	}
	for (int i = 0; i < (int)other.actionsLog.size(); i++)
	{
		actionsLog.push_back(other.actionsLog.at(i)->clone());
	}
	for (int i = 0; i < (int)other.menu.size(); i++)
	{
		menu.push_back(other.menu.at(i));
	}
	return *this;
}

Restaurant & Restaurant::operator=(Restaurant && other)
{
	if (this == &other) 
	{
		other.tables.clear();
		other.actionsLog.clear();
		return *this;
	}
	for (int i = 0; i < (int)tables.size(); i++) {
			delete tables.at(i);
	}
	for (int i = 0; i < (int)actionsLog.size(); i++) {
		delete actionsLog.at(i);
	}
	menu.clear();
	Tnumber = other.getNumOfTables();
	open = other.isOpen();
	tables = other.tables;
	for (int i = 0; i < (int)other.menu.size(); i++)
	{
		menu.push_back(other.menu.at(i));
	}
	actionsLog = other.actionsLog;
	other.tables.clear();
	other.actionsLog.clear();
	return *this;
}

Restaurant::~Restaurant()
{
	for (int i = 0; i < (int)tables.size(); i++) {
		delete tables.at(i);
	}
	for (int i = 0; i < (int)actionsLog.size(); i++) {
		delete actionsLog.at(i);
	}
}

void Restaurant::start()
{
	std::cout << "Restaurant is now open!"  << std::endl;
	bool running = true;
	while (running) 
	{
		std::string stg;
		std::getline(std::cin,stg);
		std::istringstream iss(stg);
		std::vector<std::string> breakDouwn;
		std::string input;
		while (std::getline(iss, input, ' ')) {
			breakDouwn.push_back(input);
		}
		if (breakDouwn.at(0)=="open")
		{
			std::vector<Customer*> customers;
			int id = stoi(breakDouwn.at(1));
			int i=2;
			while (i<(int)breakDouwn.size())
			{
				std::istringstream iss(breakDouwn.at(i));
				std::vector<std::string> temp;
				while (std::getline(iss, input, ',')) {
					temp.push_back(input);
				}
				Customer *c;
				if (temp.at(1) == "chp")
				{
					c = new CheapCustomer(temp.at(0), Cid);
					Cid++;
				}
				else if (temp.at(1) == "veg")
				{
					c = new VegetarianCustomer(temp.at(0), Cid);
					Cid++;
				}
				else if (temp.at(1) == "spc")
				{
					c = new SpicyCustomer(temp.at(0), Cid);
					Cid++;
				}
				else
				{
					c = new AlchoholicCustomer(temp.at(0), Cid);
					Cid++;
				}
				customers.push_back(c);
				i++;
			}
			BaseAction *act = new OpenTable(id, customers);
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (breakDouwn.at(0)=="order")
		{
			BaseAction *act = new Order(stoi(breakDouwn.at(1)));
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (breakDouwn.at(0) == "move")
		{
			std::istringstream iss(stg);
			std::vector<std::string> words;
			std::string input;
			while (std::getline(iss, input, ' ')) {
				words.push_back(input);
			}
			BaseAction *act = new MoveCustomer(stoi(words.at(1)),stoi(words.at(2)),stoi(words.at(3)));
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (breakDouwn.at(0) == "close")
		{
			std::istringstream iss(stg);
			std::vector<std::string> words;
			std::string input;
			while (std::getline(iss, input, ' ')) {
				words.push_back(input);
			}
			BaseAction *act = new Close(stoi(words.at(1)));
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if(breakDouwn.at(0) == "status")
		{
			std::istringstream iss(stg);
			std::vector<std::string> words;
			std::string input;
			while (std::getline(iss, input, ' ')) {
				words.push_back(input);
			}
			BaseAction *act = new PrintTableStatus(stoi(words.at(1)));
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (stg == "restore")
		{
			BaseAction *act = new RestoreResturant();
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (stg == "backup")
		{
			BaseAction *act = new BackupRestaurant();
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (stg == "log")
		{
			BaseAction *act = new PrintActionsLog();
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (stg == "menu")
		{
			BaseAction *act = new PrintMenu();
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (stg == "closeall")
		{
			BaseAction *act = new CloseAll();
			act->act(*this);
			actionsLog.push_back(act);
			running=false;
		}
	}
}

int Restaurant::getNumOfTables() const
{
	return Tnumber;
}

Table * Restaurant::getTable(int ind)
{
	return tables.at(ind);
}

const std::vector<BaseAction*>& Restaurant::getActionsLog() const
{
	return actionsLog;
}

std::vector<Dish>& Restaurant::getMenu()
{
	return menu;
}

bool Restaurant::isOpen()
{
	return open;
}
