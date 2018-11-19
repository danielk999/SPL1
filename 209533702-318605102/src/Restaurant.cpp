#include "Restaurant.h"
#include <iostream>     
#include <sstream>       


Restaurant::Restaurant():open(false),tables(),menu(),actionsLog(){}

Restaurant::Restaurant(const std::string & configFilePath):open(false), tables(), menu(), actionsLog()
{
	std::istringstream iss(configFilePath);
	std::vector<std::string> results;
	std::string input;
	while (std::getline(iss, input, '\n')) {
		results.push_back(input);
	}
	int count = 1;
	for (int i = 0; i < results.size(); i++)
	{
		if (results.at(i)[0] != '#' & results.at(i)!="")
		{
			if (count == 1)
			{
				int length = 0;
				for (int j = 0; j < results.at(i).size(); j++) 
				{
					if (results.at(i)[j] != ' ' & results.at(i)[j] != '\n')
					{
						length = length * 10 + (int)results.at(i)[j];
					}
					else 
					{
						break;
					}
				}
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
				for (int j = 0; j < TSeats.size() & j < Tnumber; j++)
				{
					int length = 0;
					for (int j = 0; j < results.at(i).size(); j++)
					{
						if (results.at(i)[j] != ' ' & results.at(i)[j] != '\n')
						{
							length = length * 10 + (int)results.at(i)[j];
						}
						else
						{
							break;
						}
					}
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

Restaurant::Restaurant(Restaurant & other):Tnumber(other.getNumOfTables()),open(other.isOpen()), menu(other.menu)
{
	for (int i = 0; i < other.tables.size(); i++)
	{
		tables.push_back(new Table(*other.tables.at(i)));
	}
	for (int i = 0; i < other.actionsLog.size(); i++)
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
	for (int i = 0; i < tables.size(); i++) {
		delete tables.at(i);
	}
	for (int i = 0; i < actionsLog.size(); i++) {
		delete actionsLog.at(i);
	}
	menu.clear();
	Tnumber = other.getNumOfTables();
	open = other.isOpen();
	for (int i = 0; i < other.tables.size(); i++)
	{
		tables.push_back(new Table(*other.tables.at(i)));
	}
	for (int i = 0; i < other.actionsLog.size(); i++)
	{
		actionsLog.push_back(other.actionsLog.at(i)->clone());
	}
	menu = other.menu;
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
	for (int i = 0; i < tables.size(); i++) {
			delete tables.at(i);
	}
	for (int i = 0; i < actionsLog.size(); i++) {
		delete actionsLog.at(i);
	}
	menu.clear();
	Tnumber = other.getNumOfTables();
	open = other.isOpen();
	tables = other.tables;
	menu = other.menu;
	actionsLog = other.actionsLog;
	other.tables.clear();
	other.actionsLog.clear();
	return *this;
}

Restaurant::~Restaurant()
{
	for (int i = 0; i < tables.size(); i++) {
		delete tables.at(i);
	}
	for (int i = 0; i < actionsLog.size(); i++) {
		delete actionsLog.at(i);
	}
}

void Restaurant::start()
{
	std::cout << "Restaurant is now open!" << '\n' << std::endl;
	bool running = true;
	while (running) 
	{
		std::string stg;
		std::cin >> stg;
		if (stg.substr(0, 4) == "open")
		{
			std::vector<Customer*> customers;
			stg = stg.substr(5);
			int id = 0, i = 0;
			while (i < stg.length())
			{
				id = id * 10 + (int)stg.at(i);
				i++;
			}
			stg = stg.substr(i + 2);
			while (stg.length() > 0)
			{
				i = 0;
				std::string name = "";
				while (stg.at(i) != ',')
				{
					name += stg.at(i);
					i++;
				}
				stg = stg.substr(i + 1);
				std::string style = stg.substr(0, 3);
				Customer *c;
				if (style == "chp")
				{
					c = new CheapCustomer(name, Cid);
					Cid++;
				}
				else if (style == "veg")
				{
					c = new VegetarianCustomer(name, Cid);
					Cid++;
				}
				else if (style == "spc")
				{
					c = new SpicyCustomer(name, Cid);
					Cid++;
				}
				else
				{
					c = new AlchoholicCustomer(name, Cid);
					Cid++;
				}
				customers.push_back(c);
				stg = stg.substr(0, 4);
			}
			BaseAction *act = new OpenTable(id, customers);
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (stg.substr(0,5)=="order")
		{
			stg = stg.substr(6);
			BaseAction *act = new Order(stoi(stg));
			act->act(*this);
			actionsLog.push_back(act);
		}
		else if (stg.substr(0, 4) == "menu")
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
		else if (stg.substr(0, 4) == "close")
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
		else if(stg.substr(0, 5) == "status")
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
			delete this;
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
	if (tables.size()>ind)
	{
		return tables.at(ind - 1);
	}
	return nullptr;
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
