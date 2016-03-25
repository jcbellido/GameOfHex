/** WARNING : this program use C++ 11 feature. The compiler must be set to handle this feature.
 *  This program implements Dijkstra's Algorithm.
 *  A graph  Graph (G = (V, E)) is represented here as a vector of node V and a vector of edges E.
 *  See Graph.h and Edge.h for detail. A list of edges has been preferred to an array because it is lees memory consuming.
 *  The program needs five parameters to run :
 *  - the size of the graph
 *  - the density
 *  - the low limit of the distance of an edge
 *  - the high limit of the distance of an edge
 *  - the node (a value between 0 and size) from which the shortest paths are computed
 */
#include <iostream>
#include "Edge.h"
#include "Graph.h"
#include "ShortestPath.h"

const unsigned int SIZE(50);
const unsigned int DENSITY(5);                         // 20%
const unsigned int LOW_DISTANCE_LIMIT_RANGE(1);        // the edge cost is a value between LOW_DISTANCE_LIMIT_RANGE
const unsigned int HIGH_DISTANCE_LIMIT_RANGE(10);      // and HIGH_DISTANCE_LIMIT_RANGE
const unsigned int SOURCE_NODE(0);                     // Compute all shortest path from this node

int main()
{
    ShortestPath s(SIZE, DENSITY, LOW_DISTANCE_LIMIT_RANGE, HIGH_DISTANCE_LIMIT_RANGE);
    s.printAllShortestPathFromNode(0);
    return 0;
}
