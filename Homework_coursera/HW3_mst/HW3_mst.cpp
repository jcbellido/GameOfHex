// Minimum spanning tree exercise

#include <ctime>
#include <exception>
#include <iostream>
#include <iomanip>
#include <limits>
#include <set>
#include <stack>
#include <vector>

using namespace std;
 
using vertexKey = unsigned int;

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
class Edge
{
private:
	vertexKey start;
	vertexKey end;
	weight cost;

public:
	Edge() : start(0), end(0), cost(1) { }
	Edge(vertexKey startNodeKey, vertexKey endNodeKey, weight cost) : start(startNodeKey), end(endNodeKey), cost(cost) { }

	const vertexKey Start() const { return start; }
	const vertexKey End() const { return end; }
	const weight Cost() const { return cost; }

	// TODO: remove this one? 
	bool operator== (const Edge<weight>& other) const
	{
		return this->End() == other.End();
	}

	bool operator< (const Edge<weight>& other) const
	{
		return this->End() < other.End();
	}
};

template<class weight>
class Graph
{
private:
	vector< set< Edge<weight> > > m_vertices;

public:
	Graph() { }

	// Generator that reads a file
	// TODO

	// Random generator constructor: call and populate the matrix
	Graph(vertexKey vertex_count, float edge_density, weight min_weight, weight max_weight)
	{
		if (vertex_count == 0)
			throw invalid_argument("Vertex count must be greater than zero");

		if ((0.0f >= edge_density) || (edge_density > 1.0f))
			throw invalid_argument("Edge density must be in the range (0, 1]");

		if (max_weight <= min_weight)
			throw invalid_argument("Max weight must be bigger than min weight");

		m_vertices.resize(vertex_count);

		constructGraphRepresentation(edge_density, min_weight, max_weight);
	}

	template<class weight>
	friend ostream& operator<<(ostream& cout, const Graph<weight>& g);

	bool DoesVertexExists(vertexKey key) const
	{
		return  key < static_cast<vertexKey>(m_vertices.size());
	}

	unsigned int VertexCount() const
	{
		return m_vertices.size();
	}

	weight GetEdgeCost(vertexKey origin, vertexKey destination) const
	{
		if (!DoesVertexExists(origin) || !DoesVertexExists(destination))
			throw invalid_argument("Get Edge Cost, wrong origin or destination");

		auto vertexConnections = m_vertices[origin];
		auto setIterator = vertexConnections.find(Edge<weight>(origin, destination, 0));
		if (setIterator != vertexConnections.end())
			return (*setIterator).Cost();
		else
			return numeric_limits<weight>::min();
	}

	bool IsAdjacent(vertexKey origin, vertexKey destination) const
	{
		if (!DoesVertexExists(origin) || !DoesVertexExists(destination))
			throw invalid_argument("Is adjacent, wrong origin or destination");

		if (destination == origin)
			return true;

		if (GetEdgeCost(origin, destination) > 0)
			return true;

		return false;
	}

	//const vector<vertexKey> Neighbors(vertexKey origin) const
	//{
	//	vector<vertexKey> output;
	//	for (vertexKey i = 0; i < vertex_count; i++)
	//	{
	//		if (origin != i)
	//			if (IsAdjacent(origin, i))
	//				output.push_back(i);
	//	}
	//	return output;
	//}

	//weight PathWeight(stack<vertexKey> path) const
	//{
	//	if (path.size() <= 1)
	//		return 0;

	//	weight output = 0;
	//	vertexKey origin = path.top();
	//	path.pop();
	//	vertexKey destination;
	//	do
	//	{
	//		destination = path.top();
	//		path.pop();
	//		if (!IsAdjacent(origin, destination))
	//			throw invalid_argument("Invalid edge on path");

	//		output += GetEdgeValue(origin, destination);
	//		swap(origin, destination);
	//	} while (!path.empty());
	//	return output;
	//}

private: 
	void AddEdge(vertexKey start, vertexKey end, weight cost)
	{
		m_vertices[start].insert(Edge<weight>(start, end, cost));
	}

// Random generation of the Graph
private:
	void constructGraphRepresentation(float edge_density, weight min_weight, weight max_weight)
	{
		for (vertexKey i = 0; i < VertexCount(); ++i)
		{
			for (vertexKey j = 0; j < i; j++)
			{
				//if (i == j)
				//	continue; 

				if (doesEdgeExists(edge_density))
				{
					weight cost = get_random_weight(min_weight, max_weight);
					AddEdge(i, j, cost);
					AddEdge(j, i, cost);
				}
			}
		}
	}

	inline bool doesEdgeExists(float edge_density)
	{
		return rand_zero_to_one() < edge_density;
	}

	inline weight get_random_weight(weight min_weight, weight max_weight)
	{
		weight random = static_cast<weight>(rand()) / static_cast <weight>(RAND_MAX);
		weight diff = max_weight - min_weight;
		weight r = random * diff;
		return min_weight + r;
	}
};

template<class weight>
ostream& operator<<(ostream& cout, const Graph<weight>& g)
{
	cout << "Vertex count: " << g.m_vertices.size() << endl;
	vertexKey vk = 0;
	for (auto vertexConnections : g.m_vertices)
	{
		cout << "V: " << vk << "--";
		for (set< Edge<weight> >::iterator it = vertexConnections.begin(); it != vertexConnections.end(); it++)
		{
			cout << (*it).End() << ":" << static_cast<unsigned int>((*it).Cost()) << " ; ";;
		}
		cout << endl;
		vk++;
	}
	return cout;
}


//// ----------------------------------------------------------------------------
//// Dijstra shortest path algorithm implementation 
//template<class weight>
//class NodeDistance
//{
//private:
//	vertexKey nodeIndex;
//	weight pathAggregateCost;
//
//public:
//	NodeDistance() : nodeIndex(-1), pathAggregateCost(-1) { }
//	NodeDistance(vertexKey nodeIndex, weight currentWeight) : nodeIndex(nodeIndex), pathAggregateCost(currentWeight) { }
//
//	const weight getCost() const { return pathAggregateCost; }
//	void setCost(weight newCost) { pathAggregateCost = newCost; }
//	const vertexKey getNodeIndex() const { return nodeIndex; }
//
//	bool operator== (const NodeDistance<weight>& other) const
//	{
//		return this->getNodeIndex() == other.getNodeIndex();
//	}
//
//	bool operator< (const NodeDistance<weight>& other) const
//	{
//		if (this->getCost() == other.getCost())
//			return this->getNodeIndex() < other.getNodeIndex();
//
//		return this->getCost() < other.getCost();
//	}
//};
//
//// Dijstra Algorithm 
//// Uses the variable names from the wikipedia pseudocode implementation
//// it's also the classical implementation
//template<class weight>
//class DijkstraAlgorithm
//{
//private:
//	const Graph<weight> &m_graph;
//	set<NodeDistance<weight>> m_q;
//	vector<weight> m_dist;
//	vector<int> m_prev;
//
//	const weight WINFINITY = numeric_limits<weight>::max();	// weight-infinity to avoid constant clash
//	const int UNDEFINED = numeric_limits<int>::min();
//
//public:
//	DijkstraAlgorithm(const Graph<weight> &graph) : m_graph(graph)  { }
//
//	/// Returns the shortest path for the specific algorithm
//	/// or an empty stack if no path was found
//	stack<vertexKey> GetPath(vertexKey source, vertexKey destination)
//	{
//		if (!m_graph.DoesVertexExists(source) || !m_graph.DoesVertexExists(destination))
//			throw invalid_argument("Sources or Destinations indexes not present in the graph");
//
//		if (source == destination)
//		{
//			stack<vertexKey> trivialPath;
//			trivialPath.push(source);
//			return trivialPath;
//		}
//
//		initialization(source);
//		while (!m_q.empty())
//		{
//			auto u = *m_q.begin();
//			m_q.erase(m_q.begin());
//			if (u.getNodeIndex() == destination)
//				break;
//
//			auto neighbors = m_graph.Neighbors(u.getNodeIndex());
//			for (auto v : neighbors)
//			{
//				weight alt = m_dist[u.getNodeIndex()] + m_graph.GetEdgeValue(u.getNodeIndex(), v);
//				if (alt < m_dist[v])
//				{
//					auto f = find(m_q.begin(), m_q.end(), NodeDistance<weight>(v, m_dist[v]));
//					m_q.erase(f);
//					m_q.insert(NodeDistance<weight>(v, alt));
//					m_dist[v] = alt;
//					m_prev[v] = u.getNodeIndex();
//				}
//			}
//		}
//
//		// Generate a path as result
//		stack<vertexKey> path;
//		vertexKey u = destination;
//		while ((m_prev[u] != UNDEFINED) && (u != source))
//		{
//			path.push(u);
//			u = m_prev[u];
//		}
//
//		if (u == source)
//		{
//			path.push(source);
//			return path;
//		}
//		else
//		{
//			return stack<vertexKey>();	// Empty stack : no path found
//		}
//	}
//
//private:
//	void clear()
//	{
//		m_q.clear();
//		m_dist.clear();
//		m_prev.clear();
//	}
//
//	void initialization(vertexKey source)
//	{
//		clear();
//		m_dist.resize(m_graph.VertexCount());
//		m_prev.resize(m_graph.VertexCount());
//		m_dist[source] = 0;
//		for (vertexKey i = 0; i < m_graph.VertexCount(); ++i)
//		{
//			if (i != source)
//			{
//				m_dist[i] = WINFINITY;
//				m_prev[i] = UNDEFINED;
//			}
//			m_q.insert(NodeDistance<weight>(i, m_dist[i]));
//		}
//	}
//};
//
//
//// ----------------------------------------------------------------------------
//// Assignement
//template<class weight>
//weight GetAveragePathLength(vertexKey numberOfVertex, float edgeDensity, weight minDistance, weight maxDistance)
//{
//	if (numberOfVertex <= 1)
//		return 0;
//
//	weight output = 0;
//	int numberOfPaths = 0;
//	auto graph = new Graph<weight>(numberOfVertex, edgeDensity, minDistance, maxDistance);
//	auto algorithm = new DijkstraAlgorithm<float>(*graph);
//
//	for (vertexKey i = 1; i < graph->VertexCount(); i++)
//	{
//		auto path = algorithm->GetPath(0, i);
//
//		if (!path.empty())
//		{
//			output += graph->PathWeight(path);
//			numberOfPaths++;
//		}
//	}
//
//	delete(algorithm);
//	delete(graph);
//
//	return output / static_cast<weight>(numberOfPaths);
//}

int main()
{
	// srand(static_cast<unsigned int>(time(0)));
	srand(7777);
	Graph<float> random_g = Graph<float>(5, 0.4f, 1.0f, 10.0f);
	cout << random_g << endl;

	return 0;
}
