# All Targets
all: rest

# Tool invocations
# Executable "main" depends on the files rest.o and Main.o and Customer.o and Restaurant.o and Dish.o and Table.o and Action.o.
rest: bin/Main.o bin/Customer.o bin/Restaurant.o bin/Dish.o bin/Action.o bin/Table.o
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/rest bin/Restaurant.o bin/Main.o bin/Customer.o bin/Dish.o bin/Action.o bin/Table.o
	@echo 'Finished building target: rest'
	@echo ' '

# Depends on the source and header files
bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

# Depends on the source and header files 
bin/Restaurant.o: src/Restaurant.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

# Depends on the source and header files 
bin/Dish.o: src/Dish.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Dish.o src/Dish.cpp

# Depends on the source and header files 
bin/Table.o: src/Table.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Table.o src/Table.cpp

# Depends on the source and header files 
bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files 
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

#Clean the build directory
clean: 
	rm -f bin/*
