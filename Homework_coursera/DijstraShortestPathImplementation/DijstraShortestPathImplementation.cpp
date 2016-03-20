// DijstraShortestPathImplementation.cpp : Defines the entry point for the console application.
//
// Create a graph as an abstract data type: using C++ classes
//		assume UNDIRECTED graphs
//		edge lists
//		connection matrixes
//		random graph constructor
//			edge density (0..1)
//			edge distance range as a parameter
// Dijstra's algorithm should be a class member function 

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

template<class weight>
class Graph
{
private:
	// let's use matrixes for now ... makes thing simpler ...
	vector<vector<weight>> m_matrix_weight;
public: 
	Graph(unsigned int vertex_count, float edge_density, weight min_weight, weight max_weight)
	{
		// static_assert(0.0f < edge_density <= 1.0f, "Edge should be a value between (0, 1]");
		// static_assert(max_weight > min_weight, "Weight should be a valid range, max > min");
		// static_assert(vertex_count >= 2, "Vertex count should be 2 or greater");
		// move 


	}

	~Graph()
	{
		std::cout << "Graph deletion ... " << std::endl;
	}
};

int main()
{
	cout << "Executing Dijstra shortest" << endl;
	auto g = new Graph<float>(50, 0.1f, 1.0f, 10.0f);

	return 0;
}
