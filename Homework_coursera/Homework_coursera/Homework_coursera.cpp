// Homework 1: Convert a C program to C++

// This exercise is using two aspects of C11
//   - auto to make the code les verbose
//   - for each iterator

#include <iostream>
#include <vector>

using namespace std;

const int N = 40;

// For simplicity sake this function assumes that accumulator and data share class
template <class T>
inline void sum(T& accumulator, const vector<T> &data)
{
	
	accumulator = 0;
	for (auto i : data)	// Using vectors, size is no longer neccesary
	
		accumulator += i;
}


int weird_main()
{
	auto data = vector<int>();
	for (int i = 0; i < N; ++i)
		data.push_back(i);

	int accumulator;
	sum(accumulator, data);
	cout << "sum is " << accumulator << endl;

	return 0;
}
