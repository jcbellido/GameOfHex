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

#include <ctime>
#include <exception>
#include <iostream>
#include <vector>

using namespace std;

inline float rand_zero_to_one()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline int rand_int_in_positive_range(unsigned int min, unsigned int max)
{
	return min + (rand() % (int)(max - min + 1));
}

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

		this->edge_density = edge_density;
		this->max_weight = max_weight;
		this->min_weight = min_weight;
		this->vertex_count = vertex_count;

		construct_graph_representation(min_weight, max_weight);
	}

	~Graph()
	{
		if (m_matrix_weight != nullptr)
			delete(m_matrix_weight);
	}

	void print()
	{
		for (auto it = m_matrix_weight->begin(); it != m_matrix_weight->end(); it++)
		{
			for (vector<weight>::iterator inner_it = *it->begin(); inner_it != *it->begin(); inner_it++)
			{
				cout << (*inner_it) << " ";
			}
			cout << endl;
		}
	}

private:
	vector<vector<weight>>* m_matrix_weight = nullptr;
	float edge_density = 0.0f; 
	unsigned int vertex_count = 0; 
	weight min_weight;
	weight max_weight;

	inline bool does_edge_exists()
	{
		return this->edge_density < rand_zero_to_one();
	}

	inline weight get_random_weight()
	{
		weight random = static_cast<weight>(rand()) / static_cast <weight>(RAND_MAX);
		weight diff = max_weight - min_weight;
		weight r = random * diff;
		return min_weight + r;
	}

protected: 
	virtual void construct_graph_representation(weight min_weight, weight max_weight)
	{
		m_matrix_weight = new vector<vector<weight>>(vertex_count);
	
		for (unsigned int i = 1; i < vertex_count; ++i)
			m_matrix_weight->push_back(vector<weight>(i));	// Initialize the structure

		// Traverse the lower half of the structure and then mirror it
		auto row_iterator = m_matrix_weight->begin();
		for (unsigned int i = 0; i < vertex_count; ++i)
		{
			vector<weight> row = *row_iterator;
			for (unsigned int j = 0; j < i; ++j)
			{
				if (does_edge_exists())
				{
					row.push_back(get_random_weight());
				}
				else
				{
					row.push_back(0);
				}
			}
			row_iterator++;
		}
	}
};

int main()
{
	srand(time(0));
	cout << "Executing Dijstra shortest" << endl;
	auto g = new Graph<float>(10, 0.1f, 1.0f, 10.0f);
	g->print();
	delete(g);
	return 0;
}
