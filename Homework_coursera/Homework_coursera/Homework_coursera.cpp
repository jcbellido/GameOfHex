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

void sum(int*p, int n, int d[])
{

	int i;

	*p = 0;

	for (i = 0; i < n; ++i)

		*p = *p + d[i];
}

int original_main()
{
	int data[N];
	for (int i = 0; i < N; ++i)

		data[i] = i;

	int accum = 0;

	sum(&accum, N, data);

	cout << "Sum is " << accum << endl;

	return 0;
}

// std vector tiene un iterador propio ... asi q no veo porque no usarlo ... 
template <class T, class S>
inline void sum_vector(T& accumulator, vector<S> &data)
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

	float accum = 0;
	sum_vector(accum, data);
	cout << "Sum is " << accum << endl;

	return 0;
}
