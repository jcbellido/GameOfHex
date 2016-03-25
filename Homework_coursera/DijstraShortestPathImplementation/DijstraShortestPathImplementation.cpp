// DijkstraShortestPathImplementation.cpp 

// Some notes about the graph implementation: 
// 1. I am using a matrix approach to keep the graph structure, it seemed simpler for debugging purposes
// 2. The grah is non directed
// 3. I'm not generating edges that go from a node into itself
// 4. For practice purposes I'm keeping the edge cost as a templated class

// About the Dijkstra algorithm implemented
// 1. It follows wikipedia's pseudocode and names.
// 2. Using sets as data containers.
// 3. I'm using the limits package to construct undefined and infinity values.
// 4. It's not the optimized version, this way the code is clearer. 

// Some general notes: 
// 1. I was curious about the use of exceptions in C++ and I'm using them to report bad parameters
// 2. I used cout a bit and there's some output formating (through iomanip)

#include <ctime>
#include <exception>
#include <iostream>
#include <iomanip>
#include <limits>
#include <set>
#include <stack>
#include <vector>

using namespace std;

// ----------------------------------------------------------------------------
// Auxiliar math functions
inline float rand_zero_to_one()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline int rand_int_in_positive_range(unsigned int min, unsigned int max)
{
	return min + (rand() % (int)(max - min + 1));
}

// ----------------------------------------------------------------------------
// Graph representation
template<class weight>
class Graph
{
public:
	Graph(unsigned int vertex_count, float edge_density, weight min_weight, weight max_weight)
	{
		if (vertex_count == 0)
			throw invalid_argument("Vertex count must be greater than zero");

		if ((0.0f >= edge_density) || (edge_density > 1.0f))
			throw invalid_argument("Edge density must be in the range (0, 1]");

		if (max_weight <= min_weight)
			throw invalid_argument("Max weight must be bigger than min weight");

		this->edge_density = edge_density;
		this->max_weight = max_weight;
		this->min_weight = min_weight;
		this->vertex_count = vertex_count;

		constructGraphRepresentation(min_weight, max_weight);
	}

	template<class weight>
	friend ostream& operator<<(ostream& cout, const Graph<weight>& g);

private:
	vector<vector<weight>> m_matrix_weight;
	float edge_density = 0.0f; 
	unsigned int vertex_count = 0; 
	weight min_weight;
	weight max_weight;

	virtual void constructGraphRepresentation(weight min_weight, weight max_weight)
	{
		for (unsigned int i = 1; i <= vertex_count; ++i)
			m_matrix_weight.push_back(vector<weight>(i));	// Initialize the structure

		// Traverse the lower half of the structure and then mirror it
		// Also: avoids edges that starts at ends on the same node
		auto row_iterator = m_matrix_weight.begin();
		for (unsigned int i = 0; i < vertex_count; ++i)
		{
			unsigned int j = 0;
			for (auto& column_iterator : *row_iterator)
			{
				if ((i != j) && doesEdgeExists())
				{
					column_iterator = get_random_weight();
				}
				else
				{
					column_iterator = 0;
				}
				j++;
			}
			row_iterator++;
		}
	}

	inline bool doesEdgeExists()
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

	bool DoesVertexExists(unsigned int key) const
	{
		return  key < vertex_count;
	}

	unsigned int VertexCount() const
	{
		return vertex_count;
	}

	weight GetEdgeValue(unsigned int origin, unsigned int destination) const
	{
		if (destination > origin)
			swap(origin, destination);	// Note: This implementation keeps the lower part of the matrix

		vector<weight> row = m_matrix_weight[origin];
		return row[destination];
	}
	
	bool IsAdjacent(unsigned int origin, unsigned int destination) const
	{
		if (destination == origin)
			return true;

		if (destination > origin)
			swap(origin, destination);	// Note: This implementation keeps the lower part of the matrix
		
		if (GetEdgeValue(origin, destination) > 0)
			return true;
		
		return false;
	}

	const vector<unsigned int> Neighbors(unsigned int origin) const
	{
		vector<unsigned int> output;
		for (unsigned int i = 0; i < vertex_count; i++)
		{
			if (origin != i)
				if (IsAdjacent(origin, i))
					output.push_back(i);
		}
		return output;
	}

	weight PathWeight(stack<unsigned int> path) const
	{
		if (path.size() <= 1)
			return 0;

		weight output = 0;
		unsigned int origin = path.top(); 
		path.pop();
		unsigned int destination;
		do
		{
			destination = path.top();
			path.pop();
			if (!IsAdjacent(origin, destination))
				throw invalid_argument("Invalid edge on path");

			output += GetEdgeValue(origin, destination);
			swap(origin, destination);
		} while (!path.empty());
		return output; 
	}
};

template<class weight>
ostream& operator<<(ostream& cout, const Graph<weight>& g)
{
	cout << "Edge density: " << g.edge_density << endl;
	cout << "Vertex count: " << g.vertex_count << endl;
	for (auto row : g.m_matrix_weight)
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
	return cout;
}


// ----------------------------------------------------------------------------
// Dijstra shortest path algorithm implementation 
template<class weight>
class NodeDistance
{
private: 
	unsigned int nodeIndex;
	weight pathAggregateCost;

public:
	NodeDistance() : nodeIndex(-1), pathAggregateCost(-1) { }
	NodeDistance(unsigned int nodeIndex, weight currentWeight) : nodeIndex(nodeIndex), pathAggregateCost(currentWeight) { }

	const weight getCost() const { return pathAggregateCost; }
	void setCost(weight newCost) { pathAggregateCost = newCost; }
	const unsigned int getNodeIndex() const { return nodeIndex; }

	bool operator== (const NodeDistance<weight>& other) const
	{
		return this->getNodeIndex() == other.getNodeIndex();
	}

	bool operator< (const NodeDistance<weight>& other) const
	{
		if (this->getCost() == other.getCost())
			return this->getNodeIndex() < other.getNodeIndex();

		return this->getCost() < other.getCost();
	}
};

// Dijstra Algorithm 
// Uses the variable names from the wikipedia pseudocode implementation
// it's also the classical implementation
template<class weight>
class DijkstraAlgorithm
{
private:
	const Graph<weight> &m_graph;
	set<NodeDistance<weight>> m_q;
	vector<weight> m_dist;
	vector<int> m_prev;

	const weight WINFINITY = numeric_limits<weight>::max();	// weight-infinity to avoid constant clash
	const int UNDEFINED = numeric_limits<int>::min();

public:
	DijkstraAlgorithm(const Graph<weight> &graph) : m_graph(graph)  { }

	/// Returns the shortest path for the specific algorithm
	/// or an empty stack if no path was found
	stack<unsigned int> GetPath(unsigned int source, unsigned int destination)
	{
		if (!m_graph.DoesVertexExists(source) || !m_graph.DoesVertexExists(destination))
			throw invalid_argument("Sources or Destinations indexes not present in the graph");

		if (source == destination)
		{
			stack<unsigned int> trivialPath;
			trivialPath.push(source);
			return trivialPath;
		}

		initialization(source);	
		while (!m_q.empty())
		{
			auto u = *m_q.begin();
			m_q.erase(m_q.begin());
			if (u.getNodeIndex() == destination)
				break;

			auto neighbors = m_graph.Neighbors(u.getNodeIndex());
			for (auto v : neighbors)
			{
				weight alt = m_dist[u.getNodeIndex()] + m_graph.GetEdgeValue(u.getNodeIndex(), v);
				if (alt < m_dist[v])
				{
					auto f = find(m_q.begin(), m_q.end(), NodeDistance<weight>(v, m_dist[v]));
					m_q.erase(f);
					m_q.insert(NodeDistance<weight>(v, alt));
					m_dist[v] = alt;
					m_prev[v] = u.getNodeIndex();
				}
			}
		}

		// Generate a path as result
		stack<unsigned int> path;
		unsigned int u = destination;
		while ((m_prev[u] != UNDEFINED) && (u != source))
		{
			path.push(u);
			u = m_prev[u];
		}

		if (u == source)
		{
			path.push(source);
			return path;
		}
		else
		{
			return stack<unsigned int>();	// Empty stack : no path found
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
		for (unsigned int i = 0; i < m_graph.VertexCount(); ++i)
		{
			if (i != source)
			{
				m_dist[i] = WINFINITY;
				m_prev[i] = UNDEFINED;
			}
			m_q.insert(NodeDistance<weight>(i, m_dist[i]));
		}
	}
};


// ----------------------------------------------------------------------------
// Assignement
template<class weight>
weight GetAveragePathLength(unsigned int numberOfVertex, float edgeDensity, weight minDistance, weight maxDistance)
{
	if (numberOfVertex <= 1)
		return 0;

	weight output = 0;
	int numberOfPaths = 0;
	auto graph = new Graph<weight>(numberOfVertex, edgeDensity, minDistance, maxDistance);
	auto algorithm = new DijkstraAlgorithm<float>(*graph);
	
	for (unsigned int i = 1; i < graph->VertexCount(); i++)
	{
		auto path = algorithm->GetPath(0, i);

		if (!path.empty())
		{
			output += graph->PathWeight(path);
			numberOfPaths++;
		}
	}

	delete(algorithm);
	delete(graph);

	return output / static_cast<weight>(numberOfPaths);
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	cout << "Average path length on 20% : " << GetAveragePathLength<float>(50, 0.2f, 1.0f, 10.0f) << endl;
	cout << "Average path length on 40% : " << GetAveragePathLength<float>(50, 0.4f, 1.0f, 10.0f) << endl;
	return 0;
}
  