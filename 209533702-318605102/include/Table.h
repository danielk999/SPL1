#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table {
public:
	Table(int t_capacity);
	Table(const Table& other);
	Table(Table&& other);
	int getCapacity() const;
	void addCustomer(Customer* customer);
	void removeCustomer(int id);
	Customer* getCustomer(int id);
	std::vector<OrderPair> getCustomerOrders(int id);
	void removeCustomerOrders(int id);
	void addOrders(std::vector<OrderPair> orders);
	std::vector<Customer*>& getCustomers();
	std::vector<OrderPair>& getOrders();
	void order(const std::vector<Dish> &menu);
	void openTable();
	void closeTable();
	int getBill();
	bool isOpen();
	virtual ~Table();
	Table & operator=(const Table &other);
	Table & operator=(Table &&other); 
private:
	int capacity;
	bool open;
	std::vector<Customer*> customersList;
	std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
	Dish findDish(int id, const std::vector<Dish>& menu);
};


#endif