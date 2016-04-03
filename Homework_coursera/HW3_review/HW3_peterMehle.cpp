///* Minimum spanning tree using Prims algorithms
//*
//* IMPORTANT: graph file has to in the same folder as main.cpp file
//* IMPORTANT: needs to be compiled with -std=c++11 flag
//* IMPORTANT: this file is concatenation of multiple files. Some lines needs
//*            to be commented out, so compilation works without problems
//*
//*/
//
///*****************************************************************************/
///*****************************************************************************/
///*****************************************************************************/
//
///* End node representation of the Edge in the Graph with cost included
//*
//* It is used as end node of the graph in edge lists model and as node in
//* priority queue in shortest path computation
//* Members: end node 'node' and cost of the edge where node is end node
//*
//* Example:
//*      Node node(5, 10);
//*
//* CompareNode class is used as comparator for priority queue in the
//* shortest path computation
//*/
//
///* #ifndef NODE_H */
///* #define NODE_H */
//
//class Node {
//
//private:
//	int node;
//	double cost;
//
//public:
//	Node(int node, double cost);
//	double getCost() const;
//	double getNode() const;
//};
//
//class CompareNode {
//public:
//	bool operator()(Node const& n1, Node const& n2);
//};
//
///* #endif */
//
///*****************************************************************************/
///*****************************************************************************/
///*****************************************************************************/
//
///* End node representation of the Edge in the Graph with cost included
//*
//* It is used as end node of the graph in edge lists model and as node in
//* priority queue in shortest path computation
//* Members: end node 'node' and cost of the edge where node is end node
//*
//* Example:
//*      Node node(5, 10);
//*
//* CompareNode class is used as comparator for priority queue in the
//* shortest path computation
//*/
//
///* #include "node.h" */
//
///* creates new Node */
///* Node :: (int, double) -> () */
///* int node    => actual value of the end node in the edge */
///* double cost => cost of the edge where node is the end node */
//Node::Node(int node, double cost) : node(node), cost(cost) {}
//
///* returns cost of the edge where node is the end node */
///* getCost :: () -> double */
//double Node::getCost() const
//{
//	return cost;
//}
//
///* return actual node value */
///* getCost :: () -> double */
//double Node::getNode() const
//{
//	return node;
//}
//
///* used for comparision in priority queue for shortes path computation */
///* operator() :: (Node const&, Node const&) -> bool */
///* Node const& n1 => first node to compare */
///* Node const& n2 => second node to compare */
//bool CompareNode::operator()(Node const& n1, Node const& n2)
//{
//	return n1.getCost() > n2.getCost();
//}
//
///*****************************************************************************/
///*****************************************************************************/
///*****************************************************************************/
//
///* Edge representation of the graph
//* Members: start node x, end node y and cost of the edge
//* Example:
//*      Edge edge(1, 5, 10);
//*
//* CompareEdge class is used as comparator for priority queue in the mst algorithm
//*/
//
///* #ifndef EDGE_H */
///* #define EDGE_H */
//
//#include <iostream>
//
//class Edge {
//
//private:
//	int x;
//	int y;
//	double cost;
//
//public:
//	Edge(int x, int y, double cost);
//	double getCost() const;
//	int getX() const;
//	int getY() const;
//};
//
//std::ostream& operator<<(std::ostream& out, Edge const& e);
//
//class CompareEdge {
//public:
//	bool operator()(Edge const& e1, Edge const& e2);
//};
//
//
///* #endif */
//
///*****************************************************************************/
///*****************************************************************************/
///*****************************************************************************/
//
///* Edge representation of the graph
//* Members: start node x, end node y and cost of the edge
//* Example:
//*      Edge edge(1, 5, 10);
//*
//* CompareEdge class is used as comparator for priority queue in the mst algorithm
//*/
//
//#include <iostream>
///* #include "edge.h" */
//
///* creates new Edge */
///* Edge :: (int, int double) -> () */
///* int x       => start node of the edge */
///* int y       => end node of the edge */
///* double cost => cost of the edge */
//Edge::Edge(int x, int y, double cost) : x(x), y(y), cost(cost) {}
//
///* returns cost of the edge */
///* getCost :: () -> double */
//double Edge::getCost() const
//{
//	return cost;
//}
//
///* returns start node of the edge */
///* getX :: () -> int */
//int Edge::getX() const
//{
//	return x;
//}
//
///* returns end node of the edge */
///* getY :: () -> int */
//int Edge::getY() const
//{
//	return y;
//}
//
///* print the edge */
///* operator<< :: (ostream&, Edge const&) -> bool */
///* ostream& out  => output stream */
///* Edge const& e => edge to print */
//std::ostream& operator<<(std::ostream& out, Edge const& e)
//{
//	out << "(" << e.getX() << ", " << e.getY() << ", " << e.getCost() << ")";
//	return out;
//}
//
///* used for comparision in priority queue for mst algorithms */
///* operator() :: (Edge const&, Edge const&) -> bool */
///* Edge const& n1 => first edge to compare */
///* Edge const& n2 => second edge to compare */
//bool CompareEdge::operator()(Edge const& e1, Edge const& e2)
//{
//	return e1.getCost() > e2.getCost();
//}
//
///*****************************************************************************/
///*****************************************************************************/
///*****************************************************************************/
//
///* Graph representation with random graph initialization defined by
//* density and the distance range
//*
//* Main graph representation as edge list where every node in the edge list
//* also caries cost with it for more efficient computation
//* Nodes are represented as integers from 0 to n - 1
//* Members: nodes list as vector of vector of Nodes
//*          edges list as vector of Edges
//*
//* Example:
//*      Graph graph(50, 0.2, 1.0, 10);
//*
//*/
//
///* #ifndef GRAPH_H */
///* #define GRAPH_H */
//
//#include <random>
//#include <vector>
///* #include "edge.h" */
///* #include "node.h" */
//
//class Graph {
//
//private:
//	int numNodes;
//	std::random_device rd;
//	std::vector<std::vector<Node> > nodes;
//	std::vector<Edge> edges;
//
//	std::vector<Node> neighbors(int x) const;
//	bool addUndirectedEdge(int x, int y, double cost);
//	bool addDirectedEdge(int x, int y, double cost);
//	bool randomGraph(double density, double start, double end);
//
//public:
//	Graph(int n, double density, double start, double end);
//	Graph(std::string filename);
//	int numVertices() const;
//	int numEdges() const;
//	int dijkstraShortestPath(int start, int end) const;
//	std::vector<Edge> mst(int start) const;
//};
//
///* #endif */
//
///*****************************************************************************/
///*****************************************************************************/
///*****************************************************************************/
//
///* Graph representation with random graph initialization defined by
//* density and the distance range
//*
//* Main graph representation as edge list where every node in the edge list
//* also caries cost with it for more efficient computation
//* Nodes are represented as integers from 0 to n - 1
//* Members: nodes list as vector of vector of Nodes
//*          edges list as vector of Edges
//*
//* Example:
//*      Graph graph(50, 0.2, 1.0, 10);
//*
//*/
//
//#include <iostream>
//#include <fstream>
//#include <iterator>
//#include <random>
//#include <vector>
//#include <queue>
//#include <cmath>
//#include <climits>
//#include <string>
//
///* #include "node.h" */
///* #include "edge.h" */
///* #include "graph.h" */
//
///* create new random graph with edge density and random edge costs between start and end*/
///* Graph :: (int, double, double, double) */
///* int n          => number of the nodes */
///* double density => density of the edges (0 = no edges, 1 = full connected graph) */
///* double start   => minimum cost of the edge */
///* double end     => maximum cost of the edge */
///* return         => void */
//Graph::Graph(int n, double density, double start, double end)
//	: numNodes(n), nodes(n, std::vector<Node>())
//{
//	if (! randomGraph(density, start, end))
//	{
//		std::cout << "Accepted values: Density [0, 1], start > 0, end > 0, end > start";
//		std::cout << std::endl;
//	}
//}
//
///* create new graph from a file*/
///* Graph :: (string) */
///* string filename => name of the file */
///* return          => void */
//Graph::Graph(std::string filename)
//{
//	std::ifstream file(filename);
//	std::istream_iterator<std::string> start(file), end;
//	std::vector<std::string> data(start, end);
//
//	/* first line in the file is number of nodes */
//	int n = std::stoi(data.front());
//	numNodes = n;
//
//	/* nodes vector has to be resized, so there is enough space for n nodes */
//	nodes.resize(n);
//
//	/* every line consists of triple (i, j, cost) */
//	for (auto it = data.begin() + 1; it != data.end(); it += 3)
//	{
//		int i = std::stoi(*it);
//		int j = std::stoi(*(it + 1));
//		double cost = std::stod(*(it + 2));
//
//		/* sample test file has undirected edges */
//		addDirectedEdge(i, j, cost);
//	}
//}
//
///* computes the minimum spanning tree using Prim's algorithm*/
///* mst :: (int) => int */
///* int start => node where computation starts */
///* return    => list of edges in the minimum spanning tree */
//std::vector<Edge> Graph::mst(int startNode) const
//{
//	/* set of nodes already in mst */
//	std::vector<bool> reached(numNodes, false);
//	std::vector<Edge> pickedEdges;
//	std::priority_queue<Edge, std::vector<Edge>, CompareEdge> pq;
//
//	/* start node is already in the mst */
//	/* put all edges (as Edge) from start node to its neighbors to the pq */
//	reached[startNode] = true;
//	for (Node node : neighbors(startNode))
//	{
//		int endNode = node.getNode();
//		int edgeCost = node.getCost();
//		pq.push(Edge(startNode, endNode, edgeCost));
//	}
//
//	/* spanning tree has n - 1 edges */
//	int numOfEdges = numNodes - 1;
//	while (numOfEdges > 0)
//	{
//		/* pop the next Edge with min cost */
//		Edge nextEdge = pq.top();
//		pq.pop();
//		int endNode = nextEdge.getY();
//
//		/* skip if endNode was already reached */
//		if (reached[endNode]) continue;
//
//		/* put picked node to the closed set and add edge to the mst */
//		reached[endNode] = true;
//		pickedEdges.push_back(nextEdge);
//		numOfEdges -= 1;
//
//		/* get all nodes reached from picked node */
//		for (Node node : neighbors(endNode))
//		{
//			int nextNode = node.getNode();
//			double nextCost = node.getCost();
//
//			/* add reachable node to the priority queue only if not already
//			* in the closed set */
//			if (! reached[nextNode])
//			{
//				pq.push(Edge(endNode, nextNode, nextCost));
//			}
//
//		}
//	}
//
//	return pickedEdges;
//}
//
///* computes the shortest path in graph from node start to node end */
///* dijkstraShortestPath :: (int, int) => int */
///* int start => node where computation starts */
///* int end => final destination of the computation */
///* return => the value of the shortest path from start to end or INT_MIN if no path */
//int Graph::dijkstraShortestPath(int start, int end) const
//{
//	/* set of nodes where shortest path is already computed */
//	std::vector<int> closed(numNodes, INT_MIN);
//	std::priority_queue<Node, std::vector<Node>, CompareNode> pq;
//
//	/* shortest path from start to start is 0 */
//	/* put all nodes (copies) reachable from start into the min priority queue */
//	closed[start] = 0;
//	for (Node n : neighbors(start))
//	{
//		pq.push(n);
//	}
//
//	/* continue until end node is in the closed set (path is found) and
//	* priority queue is not empty (no path => disconnected graph) */
//	while ((! pq.empty()) && closed[end] < 0)
//	{
//		/* pop the next Node and get its the node and the cost */
//		Node node = pq.top();
//		pq.pop();
//		int bestNode = node.getNode();
//		int bestCost = node.getCost();
//
//		/* put picked node to the closes set with its cost */
//		closed[bestNode] = bestCost;
//
//		/* get all nodes reached from picked node */
//		for (Node next : neighbors(bestNode))
//		{
//			int nextNode = next.getNode();
//			int nextCost = next.getCost();
//
//			/* add reachable node to the priority queue only if not already
//			* in the closed set */
//			/* for easier implementation and better performance we dont update the nodes */
//			/* but instead we push new Node with different cost to the priority queue. */
//			/* if new Node has lower cost, it will pop out sooner and then will
//			* be added to closed set */
//			if (closed[nextNode] < 0)
//			{
//				/* node is added with cost of the edge + cost of the shortest
//				* path from start to picked Node */
//				Node newNode = Node(nextNode, nextCost + closed[bestNode]);
//				pq.push(newNode);
//			}
//		}
//	}
//
//	/* if shortest path exists, then we return nonnegative number - value of the path */
//	/* if there is no shortest path, then INT_MIN will be returned because the node */
//	/* was node added to the set */
//	return closed[end];
//}
//
///* it return number of nodes */
///* numVertices :: () => int */
///* return => number of vertices */
//int Graph::numVertices() const
//{
//	return numNodes;
//}
//
///* it return number of edges */
///* numEdges :: () => int */
///* return => number of edges */
//int Graph::numEdges() const
//{
//	return edges.size();
//}
//
///* it creates directed edge */
///* addDirectedEdge :: (int, int, double) -> bool */
///* int x       => one end of the edge */
///* int y       => other end of the edge */
///* double cost => cost of the edge */
///* return      => true if edge is added */
//bool Graph::addDirectedEdge(int x, int y, double cost)
//{
//	nodes[x].push_back(Node(y, cost));
//	edges.push_back(Edge(x, y, cost));
//	return true;
//}
//
///* it creates undirected edge */
///* addUndirectedEdge :: (int, int, double) -> bool */
///* int x       => one end of the edge */
///* int y       => other end of the edge */
///* double cost => cost of the edge */
///* return      => true if edge is added */
//bool Graph::addUndirectedEdge(int x, int y, double cost)
//{
//	nodes[x].push_back(Node(y, cost));
//	nodes[y].push_back(Node(x, cost));
//	edges.push_back(Edge(x, y, cost));
//	return true;
//}
//
///* it returns the vector of nodes that node is connected to */
///* neighbors :: (int) => vector<Node> */
///* int x  => node to find neighbours of */
///* return => vector of nodes connected to */
//std::vector<Node> Graph::neighbors(int x) const
//{
//	return nodes[x];
//}
//
///* it creates random graph defined by density and distance range */
///* randomGraph :: (double, double, double) -> bool */
///* double density => edge density of the graph */
///* double start   => start value of the distance range */
///* double end     => end value of the distance range */
///* return         => true if succeeded, false otherwise */
//bool Graph::randomGraph(double density, double start, double end)
//{
//	/* preconditions for valid density and range values */
//	if (density < 0 || density > 1) return false;
//	if (start < 0 || end < 0 || end < start) return false;
//
//	/* c++11 random generator */
//	std::mt19937 gen(rd());
//	std::uniform_real_distribution<double> randomDensity(0, 1);
//	std::uniform_real_distribution<double> randomCost(start, end);
//
//	for (int x = 0; x < numNodes; x += 1)
//	{
//		for (int y = x + 1; y < numNodes; y += 1)
//		{
//			if (randomDensity(gen) < density)
//			{
//				addUndirectedEdge(x, y, randomCost(gen));
//			}
//		}
//	}
//
//	return true;
//}
//
///*****************************************************************************/
///*****************************************************************************/
///*****************************************************************************/
//
///* Minimum spanning tree using Prim's algorithm
//*
//* It runs the algorithms on the file provides, like example file on
//* the website
//*
//*/
//
//#include <iostream>
//#include <iomanip>
//#include <vector>
///* #include "graph.h" */
//
//inline double getTotal(std::vector<Edge>& edges);
//inline void printEdges(std::vector<Edge>& edges);
//
///* runs the required simulations */
//int main()
//{
//	std::cout << "trying to read sample.txt" << std::endl;
//	Graph graph("sample.txt");
//	std::cout << "Number of nodes: " << graph.numVertices() << std::endl;
//	std::cout << "Number of edges: " << graph.numEdges() << std::endl;
//	std::cout << std::endl;
//
//	/* print total costs starting from all nodes */
//	for (int i = 0; i < graph.numVertices(); i += 1)
//	{
//		std::vector<Edge> mstEdges(graph.mst(i));
//		double totalCost = getTotal(mstEdges);
//		std::cout << "MST total cost from node " << std::setw(2) << i << ": " << std::flush;
//		std::cout << std::setw(4) << totalCost << std::endl;
//	}
//
//	/* print edges from particular starting node */
//	int start(5); // change it for different result
//	std::cout << std::endl;
//	std::cout << "Spanning tree edges starting from node " << start << ":" << std::endl;
//	std::vector<Edge> mstEdges(graph.mst(start));
//	printEdges(mstEdges);
//}
//
///* getTotal :: (vector<Edge>&) -> double */
///* it return sum of the costs of edges */
///* vector<Edge>& edges => list of edges */
///* return              => sum of edge costs*/
//double getTotal(std::vector<Edge>& edges)
//{
//	double totalCost(0);
//	for (auto edge : edges)
//	{
//		totalCost += edge.getCost();
//	}
//	return totalCost;
//}
//
///* printEdges :: (vector<Edge>&) -> () */
///* ir prints the edges */
///* vector<Edge>& edges => list of edges */
///* return              => void*/
//void printEdges(std::vector<Edge>& edges)
//{
//	for (Edge edge : edges)
//	{
//		std::cout << edge << " ";
//	}
//	std::cout << std::endl;
//}
