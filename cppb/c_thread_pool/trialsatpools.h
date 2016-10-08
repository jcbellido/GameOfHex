#pragma once

#include <iostream>
#include <string>
#include <tuple> 

using namespace std;

typedef tuple<double, unsigned int> MoveResult;

bool TupleCompare(const MoveResult &a, const MoveResult &b)
{
	return std::get<0>(a) < std::get<0>(b);
}

// Using this function has no problems in terms of paralellization given that is just a function
void my_first(int id)
{
	std::cout << "JCB: my firty funcion id:" << id << std::endl;
}

// now with an argument
void my_second_with_parameters(int id, int something)
{
	std::cout << "JCB: my second function id:" << id << " with parameter " << something << std::endl;
}

// let's work with return values for functions ... 
int my_third_with_return_values(int id, int argument)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "JCB: my third function id:" << id << " with parameter " << argument << std::endl;
	return argument + 1;
}

// Let's try this ... in a greater scale ... let's write on a shared object
void my_fourth_function_with_return_value(int id, int index, std::vector<MoveResult> *output_result)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "JCB: my third function id:" << id << " with index " << index << std::endl;
	auto element = output_result->begin() + index;
	std::get<0>(*element) = 2.5;
	std::get<1>(*element) = 777;
}

class Fifth
{
private:
	int increase = 0;
	int stored_value = 0;

public: 
	Fifth(int incr, int value) 
	{
		std::cout << "Fifth constructor" << std::endl; increase= incr; stored_value = value;
	}
	Fifth(const Fifth &f) { std::cout << "Fifth copy constructor" << std::endl; increase = f.increase; stored_value = f.stored_value; }
	Fifth(Fifth && f) { std::cout << "Fifth move   constructor" << std::endl; increase = f.increase; stored_value = f.stored_value; }
	void operator() (int id) const
	{
		std::cout << "JCB: fifth execution, id " << id << " result value: " << stored_value + increase << std::endl;
	}
};

