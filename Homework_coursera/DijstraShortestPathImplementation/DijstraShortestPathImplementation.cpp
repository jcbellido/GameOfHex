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
#include <iomanip>
#include <queue>
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

	// Convenience debug function
	void print()
	{
		cout << "Edge density: " << edge_density << endl;
		cout << "Vertex count: " << vertex_count << endl; 
		for (auto row : *m_matrix_weight)
		{
			for (auto column : row)
			{
				if (column == 0)
					cout << "0.0 ";
				else
					cout << setprecision(2) << column << " ";
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
		return rand_zero_to_one() < this->edge_density;
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
		m_matrix_weight = new vector<vector<weight>>();
	
		for (unsigned int i = 1; i <= vertex_count; ++i)
			m_matrix_weight->push_back(vector<weight>(i));	// Initialize the structure

		// Traverse the lower half of the structure and then mirror it
		auto row_iterator = m_matrix_weight->begin();
		for (unsigned int i = 0; i < vertex_count; ++i)
		{
			for (auto& column_iterator : *row_iterator)
			{
				if (does_edge_exists())
				{
					column_iterator = get_random_weight();
				}
				else
				{
					column_iterator = 0;
				}
			}
			row_iterator++;
		}
	}

	weight get_edge_value(unsigned int origin, unsigned int destination)
	{
		if (destination > origin)
			swap(origin, destination);	// Note: This implementation keeps the lower part of the matrix

		return *(m_matrix_weight[origin][destination]);
	}
	
	bool adjacent(unsigned int origin, unsigned int destination)
	{
		if (destination == origin)
			return true;

		if (destination > origin)
			swap(origin, destination);	// Note: This implementation keeps the lower part of the matrix
		
		if (get_edge_value(origin, destination) > 0)
			return true;
	}

	const vector<unsigned int> neighbors(unsigned int origin)
	{
		vector<unsigned int> output;
		for (unsigned int i = 0; i < vertex_count; i++)
		{
			if (origin != i)
				if (adjacent(origin, i))
					output.push_back(i);
		}
		return output;
	}
};


template<class weight>
class NodeDistance
{
private: 
	const int nodeIndex;
	weight pathAggregateCost;

public:
	NodeDistance(int nodeIndex, weight currentWeight) : nodeIndex(nodeIndex), pathAggregateCost(currentWeight) { }

// properties
public: 
	const weight getCost() { return pathAggregateCost; }
	void setCost(weight newCost) { pathAggregateCost = newCost; }
	const int getNodeIndex() { return nodeIndex; }
};

template<class weight>
class DijkstraAlgorithm
{
private:
	const Graph<weight> &m_graph;
	priority_queue<NodeDistance<weight>> m_open;
	priority_queue<NodeDistance<weight>> m_closed;

public:
	DijkstraAlgorithm(const Graph<weight> &graph) : m_graph(graph) 
	{	

	}

	/// Returns the shortest path for the specific algorithm
	void SolveAlgorithm(int source, int destination)
	{
		cout << "solving " << source << " to " << destination << endl;
	}
};

int main()
{
	srand(time(0));
	cout << "Executing Dijstra shortest" << endl;
	auto graph = new Graph<float>(10, 0.5f, 1.0f, 10.0f);
	auto algorithm = new DijkstraAlgorithm<float>(*graph);
	graph->print();
	algorithm->SolveAlgorithm(0, 5);
	delete(algorithm);
	delete(graph);
	return 0;
}
