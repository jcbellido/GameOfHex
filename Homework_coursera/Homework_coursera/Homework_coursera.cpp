// Convert this program to C++
//
// change to C++ io
//
// change to one line comments
//
// change defines of constants to const
//
// change array to vector<>
//
// inline any short function

#include <iostream>
#include <vector>

using namespace std;

const int N = 40;

// approach one: keeping the structure from the Homework
template <class T, class S>
inline void sum(T& accumulator, vector<S> &data)
{
	accumulator = 0;
	for (auto i = data.begin(); i != data.end(); i++)
		accumulator += *i;
}

int main()
{
	auto data = vector<int>();
	for (int i = 0; i < N; ++i)
		data.push_back(i);

	float accum;
	sum(accum, data);
	cout << "Sum is " << accum << endl;

	return 0;
}
