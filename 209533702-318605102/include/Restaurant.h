#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant {
public:
	Restaurant();
	Restaurant(const std::string &configFilePath);
	Restaurant(Restaurant & other);
	Restaurant(Restaurant && other);
	Restaurant & operator= (Restaurant & other);
	Restaurant & operator= (Restaurant && other);
	virtual ~Restaurant();
	void start();
	int getNumOfTables() const;
	Table* getTable(int ind);
	std::string toString();
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
	std::vector<Dish>& getMenu();
	bool isOpen();

private:
	int Did = 0;
	int Cid = 0;
	int Tnumber=0;
	bool open;
	std::vector<Table*> tables;
	std::vector<Dish> menu;
	std::vector<BaseAction*> actionsLog;
};

#endif
