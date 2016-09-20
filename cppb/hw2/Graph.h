#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

using vertexKey = unsigned int;		// convenience purposes only

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Graph declaration
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

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

	template<class weight>
	friend ostream& operator<<(ostream& cout, const Edge<weight>& e);
};

template<class weight>
struct CompareByWeight {
	bool operator() (const Edge<weight>& a, const Edge<weight>& b) const
	{
		return a.Cost() < b.Cost();
	}
};

template<class weight>
ostream& operator<<(ostream& cout, const Edge<weight>& e)
{
	cout << "{" << e.Start() << " to " << e.End() << " : " << e.Cost() << " }" << endl;
	return cout;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Graph declaration
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

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

			while (!myfile.eof())
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

		ConstructRandomGraph(edge_density, min_weight, max_weight);
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

private:
	void AddEdge(vertexKey start, vertexKey end, weight cost)
	{
		m_vertices[start].insert(Edge<weight>(start, end, cost));
	}

	// Random generation of the Graph
private:
	void ConstructRandomGraph(float edge_density, weight min_weight, weight max_weight)
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
