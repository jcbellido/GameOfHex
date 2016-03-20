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

#include <exception>
#include <iostream>
#include <vector>

using namespace std;

template<class weight>
class Graph
{
public: 
	Graph(unsigned int vertex_count, float edge_density, weight min_weight, weight max_weight)
	{
		if (vertex_count == 0)
			throw invalid_argument("Vertex count must be bigger than zero");

		if ((0.0f >= edge_density) || (edge_density > 1.0f))
			throw invalid_argument("Edge density must be in the range (0, 1]");

		if (max_weight <= min_weight)
			throw invalid_argument("Max weight must be bigger than min weight");
		construct_graph_representation(vertex_count, edge_density, min_weight, max_weight);
	}

	~Graph()
	{
		if (m_matrix_weight != nullptr)
			delete(m_matrix_weight);
	}

private:
	vector<vector<weight>>* m_matrix_weight = nullptr;

protected: 
	virtual void construct_graph_representation(unsigned int vertex_count, float edge_density, weight min_weight, weight max_weight)
	{
		m_matrix_weight = new vector<vector<weight>>(vertex_count);
		
		for (unsigned int i = 0; i < vertex_count; ++i)
			m_matrix_weight->push_back(vector<weight>(vertex_count));	// Initialize the structure

		for (unsigned int i = 0; i < vertex_count; ++i)
			for (unsigned int j = 0; j < vertex_count; ++j)
			{
				cout << std::rand() << endl;
			}
	}
};

int main()
{
	cout << "Executing Dijstra shortest" << endl;
	auto g = new Graph<float>(50, 0.1f, 1.0f, 10.0f);
	delete(g);
	return 0;
}
