#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream> // For input /output functions
#include <set>
#include <map>   // For Map functions
#include <algorithm>
using namespace std;

struct edge{                        //We will make a edge by using vertex1 ,vertex2 and weight.
	char vertex1;
	char vertex2;
	int weight;
	edge(char v1, char v2, int w)             //A constructor edge is made to create a edge having vertex1,vertex2 and weight.
	{
		vertex1 = v1;
		vertex2 = v2;
		weight = w;
	}
};

struct graph{                             //We will make a graph by using vertices and edges
	vector<char> vertices;
	vector<edge> edges;
};


// This is the datastructure of disjoint set. 
map<char, char> parent;               //parent to record the parental relation between vertices.                
map<char, int> myrank;                   //The purpose of the rank is to flatten the tree.                


char Find(char vertex)                        //Find function is used to find the required vertex
{
	if (parent[vertex] == vertex)
		return parent[vertex];

	else
		return Find(parent[vertex]);

}

void Union(char root1, char root2){                // Union function is for the union of the disjoint vertices.
	if (myrank[root1] > myrank[root2])
	{
		parent[root2] = root1;
	}

	else if (myrank[root2] > myrank[root1]){
		parent[root1] = root2;
	}

	else{
		parent[root1] = root2;
		myrank[root2]++;

	}

}

void MakeSet(char vertex){      // Makeset function makes the set of the disjoint vertices
	parent[vertex] = vertex;
	myrank[vertex] = 0;

}


void Kruskal(graph &g)           // Kruskal function is used to implement the Kruskal algorithm.
{
	vector<edge> A;                   //vector A will be used to store all the edges of the minimum spanning tree
	for (auto c : g.vertices)
	{
		MakeSet(c);
	}

	sort(g.edges.begin(), g.edges.end(), [](edge x, edge y)
	{return x.weight < y.weight; });     //To sort the edges in the ascending order according to their weight

	for (edge e : g.edges){
		char root1 = Find(e.vertex1);                  //Finding the root1 of first vertex1 of edge
		char root2 = Find(e.vertex2);                  //Finding the root2 of second vertex2 of edge  
		if (root1 != root2){
			A.push_back(e);                               //If root1 and root2 are not equal then push the edge into A
			Union(root1, root2);                          //This will merge disjoint set of vertex1 and vertex2 in 1 disjoint set
		}
	}
	for (edge e : A){
		cout << e.vertex1 << " -- " << e.vertex2 << "  " << e.weight << endl;    //To output the minimum spanning tree
	}
}



int main()
{
	cout << "meshewari code" << endl;

	char t[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	graph g;             //initializing graph
	g.vertices.push_back(t[0]);                            //initialzing the vertices of the graph
	g.vertices.push_back(t[1]);
	g.vertices.push_back(t[2]);
	g.vertices.push_back(t[3]);
	g.vertices.push_back(t[4]);
	g.vertices.push_back(t[5]);

	g.edges.push_back(edge('a', 'b', 3));                 //initializing the edges of the graph
	g.edges.push_back(edge('a', 'f', 1));
	g.edges.push_back(edge('f', 'b', 4));
	g.edges.push_back(edge('c', 'b', 5));
	g.edges.push_back(edge('c', 'f', 0));
	g.edges.push_back(edge('f', 'e', 3));
	g.edges.push_back(edge('d', 'e', 1));
	g.edges.push_back(edge('d', 'c', 2));


	Kruskal(g);     //We are calling the Kruskal function passing graph g as a parameter

	return 0;

}



