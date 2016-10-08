#include "ctpl_stl.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "trialsatpools.h"

using namespace std;

double rand_zero_to_one()
{
	return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

unsigned int rand_int_in_positive_range(unsigned int min, unsigned int max)
{
	return min + (rand() % (int)(max - min + 1));
}


void first(int id) {
    std::cout << "hello from " << id << ", function" << std::endl;
}

void aga(int id, int par) {
    std::cout << "hello from " << id << ", function with parameter " << par << std::endl;
}

struct Third {
    Third(int v) { this->v = v; std::cout << "Third ctor " << this->v << '\n'; }
    Third(Third && c) { this->v = c.v; std::cout<<"Third move ctor\n"; }
    Third(const Third & c) { this->v = c.v; std::cout<<"Third copy ctor\n"; }
    ~Third() { std::cout << "Third dtor\n"; }
    int v;
};

void mmm(int id, const std::string & s) {
    std::cout << "mmm function " << id << ' ' << s << '\n';
}

void ugu(int id, Third & t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "hello from " << id << ", function with parameter Third " << t.v <<'\n';
}


int main(int argc, char **argv) {
	std::cout << "Begining of the execution" << std::endl;

	ctpl::thread_pool p(6);	
	p.push(my_first);
	std::future<void> qw = p.push(std::ref(first));
   
	std::future<int> augmented_value = p.push(my_third_with_return_values, 100);
	
	p.push(my_second_with_parameters,  77);

	p.push(my_second_with_parameters, 999);
	
	const unsigned int size = 100;
	auto result_holder = std::vector<MoveResult>(size);
	for (auto i = result_holder.begin(); i != result_holder.end(); i ++)
	{
		std::get<0>(*i) = -0.5;
		std::get<1>(*i) = 7;
	}

	for (int i = 0; i < size; i++)
		p.push(my_fourth_function_with_return_value, i, &result_holder);

	for (int i = 0; i < size; i++)
	{
		auto f = Fifth(i, 100);
		p.push(std::move(f));
	}

	std::cout << "Returned value in the future " << augmented_value.get() << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	return 0;
}


void rest_of_the_samples()
{	
	ctpl::thread_pool p(6);	// the model I've been looking at 
	p.push(first);  // function
	p.push(aga, 7);  // function

	{
		struct Second {
			Second(const std::string & s) { std::cout << "Second ctor\n"; this->s = s; }
			Second(Second && c) { std::cout << "Second move ctor\n"; s = std::move(c.s); }
			Second(const Second & c) { std::cout << "Second copy ctor\n"; this->s = c.s; };
			~Second() { std::cout << "Second dtor\n"; }
			void operator()(int id) const {
				std::cout << "hello from " << id << ' ' << this->s << '\n';
			}
		private:
			std::string s;
		} second(", functor");

		p.push(std::ref(second));  // functor, reference
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		p.push(const_cast<const Second &>(second));  // functor, copy ctor
		p.push(std::move(second));  // functor, move ctor
		if (p.size() == p.n_idle())
			std::cout << "The queue is idling" << std::endl;
		else
			std::cout << "The queue is occupied" << std::endl;
		p.push(second);  // functor, move ctor
		p.push(Second(", functor"));  // functor, move ctor
	}
	{
		Third t(100);

		p.push(ugu, std::ref(t));  // function. reference
		p.push(ugu, t);  // function. copy ctor, move ctor
		p.push(ugu, std::move(t));  // function. move ctor, move ctor

	}
	p.push(ugu, Third(200));  // function



	std::string s = ", lambda";
	p.push([s](int id) {  // lambda
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		std::cout << "hello from " << id << ' ' << s << '\n';
	});

	p.push([s](int id) {  // lambda
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		std::cout << "hello from " << id << ' ' << s << '\n';
	});

	p.push(mmm, "worked");

	auto f = p.pop();
	if (f) {
		std::cout << "poped function from the pool ";
		f(0);
	}
	// change the number of treads in the pool

	p.resize(1);

	std::string s2 = "result";
	auto f1 = p.push([s2](int) {
		return s2;
	});
	// other code here
	//...
	std::cout << "returned " << f1.get() << '\n';

	auto f2 = p.push([](int) {
		throw std::exception();
	});
	// other code here
	//...
	try {
		f2.get();
	}
	catch (std::exception & e) {
		std::cout << "caught exception\n " << e.what() << std::endl;
	}

	// get thread 0
	auto & th = p.get_thread(0);

	// Populate a 10.000 element vector because fuck it
	//unsigned int requested_size = 15; 
	//vector<MoveResult> result_vector(requested_size);
	//for(unsigned int index = 0; index < requested_size; index ++)
	//{
	//	MoveResult m = MoveResult(rand_zero_to_one(), rand_int_in_positive_range(0, 200));
	//	result_vector[index] = m;
	//}

	//sort(begin(result_vector), end(result_vector), TupleCompare);

	std::cout << "End of the execution" << std::endl;
	if (p.size() == p.n_idle())
		std::cout << "The queue is idling" << std::endl;

}
