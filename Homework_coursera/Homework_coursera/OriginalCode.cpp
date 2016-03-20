// --------------------------------------------------
// Coursera Homework 1
//
// Author: Stefan Woehrer
// Last Modified: 2016-03-17
//
// Compiled with CMake/g++ using the Clion IDE
//
// Functionality:
//  Calculates the sum of the first CNT_ADDENDS integers
//
// Assignment description:
//  Convert this program to C++
//  change to C++ io
//  change to one line comments
//  change defines of constants to const
//  change array to vector<>
//  inline any short function
// --------------------------------------------------

#include <iostream>
#include <vector>

using namespace std;

// Number we need to sum up to. Used constexpr instead of const, because this value is known at compile time.
const int CNT_ADDENDS = 40;

// --------------------------------------------------
// Calculates the sum of all elements of int vector "addends" and writes
// the sum into the byref param "sum"
// --------------------------------------------------
inline void sum(int& sum, const vector<int>& addends)
{
	sum = 0;
	for (unsigned int i = 0; i < addends.size(); i++) {
		sum += addends[i];
	}
}

// --------------------------------------------------
// Main
// Calculates the sum and prints it on the screen.
// --------------------------------------------------
int main()
{
	vector<int> data{ CNT_ADDENDS };
	for (int i = 0; i < CNT_ADDENDS; i++) {
		data.push_back(i);
	}

	int accum = 0;
	sum(accum, data);
	cout << "cmake clion sum is " << accum << endl;
	return 0;
}

