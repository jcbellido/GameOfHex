// Minimum spanning tree exercise

#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <set>
#include <stack>
#include <string>
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

	bool operator< (const Edge<weight>& other) const
	{
		return this->End() < other.End();
	}

	// DELETEME? 
	bool InternalCompareByWeight(const Edge<weight>& a, const Edge<weight>& b) const
	{
		return a.Cost() < b.Cost();
	}
};

template<class weight>
struct CompareByWeight{
	bool operator() (const Edge<weight>& a, const Edge<weight>& b) const
	{
		return a.Cost() < b.Cost();
	}
};

template<class weight>
class Graph
{
private:
	vector< set< Edge<weight> > > m_vertices;

public:
	Graph() { }

	// Constructs the graph from a file
	Graph(string filePath)
	{
		string line;
		ifstream myfile(filePath);
		if (myfile.is_open())
		{
			unsigned int vertexCount; 
			myfile >> vertexCount;
			
			if (vertexCount == 0)
			{
				myfile.close();
				throw invalid_argument("Vertex count in file must be greater than zero");
			}

			m_vertices.resize(vertexCount);

			while(!myfile.eof())
			{
				vertexKey start, end;
				weight cost;
				myfile >> start;
				myfile >> end;
				myfile >> cost;
				
				if ((end >= VertexCount()) || (start >= VertexCount()))
					throw invalid_argument("File contains vertex indexes beyond the limit file must be greater than zero");

				AddEdge(start, end, cost);
			}
			myfile.close();
		}
		else
		{
			throw invalid_argument("File not found");
		}
	}

	// Random generator constructor
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

	// The queried key is inside a valid range
	bool DoesVertexExists(vertexKey key) const
	{
		return  key < static_cast<vertexKey>(m_vertices.size());
	}
	
	// Returns the total count of keys
	unsigned int VertexCount() const
	{
		return m_vertices.size();
	}

	// Returns the weight of an existing edge between origin and destination or weight::min (usually < 0) if it doesn't
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

	const vector<vertexKey> Neighbors(vertexKey origin) const
	{
		if (!DoesVertexExists(origin))
			throw invalid_argument("Neightbors origin vertex is wrong");
		
		vector<vertexKey> output;
		auto vertexConnections = m_vertices[origin];
		for (set< Edge<weight> >::iterator edge = vertexConnections.begin(); edge != vertexConnections.end(); edge++)
		{
			output.push_back((*edge).End());
		}
		return output;
	}

	const set<Edge<weight>> EdgesFromNode(vertexKey origin) const
	{
		if (!DoesVertexExists(origin))
			throw invalid_argument("Edges From Node origin vertex is wrong");
		
		return m_vertices[origin];
	}

	weight PathWeight(stack<vertexKey> path) const
	{
		if (path.size() <= 1)
			return 0;

		weight output = 0;
		vertexKey origin = path.top();
		path.pop();
		vertexKey destination;
		do
		{
			destination = path.top();
			path.pop();
			if (!IsAdjacent(origin, destination))
				throw invalid_argument("Invalid edge on path");

			output += GetEdgeValue(origin, destination);
			swap(origin, destination);	// compute the next position in the path stack
		} while (!path.empty());
		return output;
	}

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

// ----------------------------------------------------------------------------
template<class weight>
class PrimSolver
{
private:
	const Graph<weight>& m_sourceGraph;
	set<vertexKey> m_closedVertex;
	vertexKey m_startingVertex; 
	set<Edge<weight>, CompareByWeight<weight> > m_availableEdges;
	set<Edge<weight>> m_choosenEdges;
public:
	// Obtain the MST for this graph
	PrimSolver(const Graph<weight>& g, vertexKey startingVertex) : m_sourceGraph(g), m_startingVertex(startingVertex)
	{	
		if (!m_sourceGraph.DoesVertexExists(startingVertex))
			throw invalid_argument("Prim Solver, starting Vertex not found");
	}

	void ComputePrim()
	{
		// Get the EDGES for starting vertex 
		m_closedVertex.insert(m_startingVertex);
		AddEdgesFromNode(m_startingVertex);
		while (!m_availableEdges.empty())
		{
			set<Edge<weight>, CompareByWeight<weight>>::iterator it = m_availableEdges.begin();
			if (m_closedVertex.find((*it).End()) == m_closedVertex.end())	// this is a new vertex
			{
				m_choosenEdges.insert((*it));
				vertexKey newVertex = (*it).End();
				m_closedVertex.insert(newVertex);
				
				if (m_closedVertex.size() == m_sourceGraph.VertexCount())	// check termination
					break;
				m_availableEdges.erase(it);
				AddEdgesFromNode(newVertex);
			}
			else
			{
				m_availableEdges.erase(it);	// we have the destination vertex in our span tree --> remove this edge
			}
		}
	}

private: 
	void AddEdgesFromNode(vertexKey key)
	{
		auto edges_from_node = m_sourceGraph.EdgesFromNode(key);
		for (auto e : edges_from_node)
		{ 
			if (m_closedVertex.find(e.End()) == m_closedVertex.end())
				m_availableEdges.insert(e);
		}
	}

public: 
	template<class weight>
	friend ostream& operator<<(ostream& cout, const PrimSolver<weight>& p);
};

template<class weight>
ostream& operator<<(ostream& cout, const PrimSolver<weight>& p)
{
	cout << "Prim solver output: not yet implemented" ; 
	return cout; 
}

// ----------------------------------------------------------------------------
int main()
{
	// srand(static_cast<unsigned int>(time(0)));
	srand(7777);

	cout << "No more randomness the graph read from a file:"<< endl;
	Graph<float> from_file_g = Graph<float>("debug_data.txt");
	cout << from_file_g << endl;
	PrimSolver<float> p = PrimSolver<float>(from_file_g, 0);
	p.ComputePrim();
	cout << p << endl;
	return 0;
}
