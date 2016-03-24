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
#include <limits>
#include <set>
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

	bool DoesVertexExists(unsigned int key) const
	{
		return  key <= vertex_count;
	}

	unsigned int VertexCount() const
	{
		return vertex_count;
	}

private:
	vector<vector<weight>>* m_matrix_weight = nullptr;
	float edge_density = 0.0f; 
	unsigned int vertex_count = 0; 
	weight min_weight;
	weight max_weight;

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

public: 
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
	unsigned int nodeIndex;
	weight pathAggregateCost;

public:
	NodeDistance() : nodeIndex(-1), pathAggregateCost(-1) { }
	NodeDistance(unsigned int nodeIndex, weight currentWeight) : nodeIndex(nodeIndex), pathAggregateCost(currentWeight) { }

// properties
public: 
	const weight getCost() const { return pathAggregateCost; }
	void setCost(weight newCost) { pathAggregateCost = newCost; }
	const unsigned int getNodeIndex() const { return nodeIndex; }

public:
	bool operator== (const NodeDistance<weight>& other) const
	{
		return this->getNodeIndex() == other.getNodeIndex();
	}

	bool operator< (const NodeDistance<weight>& other) const
	{
		return this->getCost() < other.getCost();
	}
};

template<class weight>
class DijkstraAlgorithm
{
private:
	const Graph<weight> &m_graph;
	set<NodeDistance<weight>> m_q;
	vector<weight> m_dist;
	vector<weight> m_prev;

public:
	DijkstraAlgorithm(const Graph<weight> &graph) : m_graph(graph)  { }

	/// Returns the shortest path for the specific algorithm
	void SolveAlgorithm(unsigned int source, unsigned int destination)
	{
		if (!m_graph.DoesVertexExists(source) || !m_graph.DoesVertexExists(destination))
			throw invalid_argument("Sources or Destinations indexes not present in the graph");

		initialization(source);
		cout << "solving " << source << " to " << destination << endl;
		// auto f = find(m_open.begin(), m_open.end(), NodeDistance<weight>(1, 0.75f));
		while (!m_q.empty())
		{


			//14     while Q is not empty :                              // The main loop
			//15         u ← Q.extract_min()                            // Remove and return best vertex
			//16         for each neighbor v of u :                       // only v that is still in Q
			//17             alt = dist[u] + length(u, v)
			//18             if alt < dist[v]
			//19                 dist[v] ← alt
			//20                 prev[v] ← u
			//21                 Q.decrease_priority(v, alt)
		}
	}

private:
	void clear()
	{
		m_q.clear();
		m_dist.clear();
		m_prev.clear();
	}

	void initialization(unsigned int source)
	{
		clear();
		m_dist.resize(m_graph.VertexCount());
		m_prev.resize(m_graph.VertexCount());
		m_dist[source] = 0;
		for (int i = 0; i < m_graph.VertexCount(); ++i)
		{
			if (i != source)
			{
				m_dist[i] = numeric_limits<weight>::max();
				m_prev[i] = numeric_limits<weight>::min();
			}
			m_q.insert(NodeDistance<weight>(i, m_dist[i]));
		}
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
