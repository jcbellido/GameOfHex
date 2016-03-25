#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H
#include <ctime>
#include <vector>
#include "Graph.h"
#include "Open.h"
#include "Close.h"
#include "Edge.h"

class ShortestPath
{
  public:
    // constructor
    ShortestPath(unsigned int x_size = 40, unsigned int x_densityPercentage = 20, unsigned int x_distanceRangeLowLimit = 1, unsigned int x_distanceRangeHighLimit = 10): m_graph(), m_open(), m_close(), m_shortestPath({}), m_size(x_size), m_densityPercentage(x_densityPercentage), m_distanceRangeLowLimit(x_distanceRangeLowLimit), m_distanceRangeHighLimit(x_distanceRangeHighLimit){generateGraph();}

    //destructor
    virtual ~ShortestPath(){m_shortestPath.clear();}

    /** \brief Compute the distance of the shortest path from x_source node to & x_destination node. Return true if there is a path from x_source node to & x_destination node.
     * \fn bool path_size(node const& x_source, node const& x_destination, distance& x_cost)
     * \param [in] x_source The source node
     * \param [in] x_destination The destination node
     * \param [out] x_cost The distance of the shortest path
     * \return bool
     *
     */
    bool path_size(node const& x_source, node const& x_destination, distance& x_cost);

    /** \brief Compute all shortest path from a given source node
     * \fn void printAllShortestPathFromNode(node x_source)
     * \param [in] x_source The source node
     * \return void
     *
     */
    void printAllShortestPathFromNode(node x_source);

  protected:
  private:
    Graph m_graph;                          //The graph
    Open m_open;                            //The open set
    Close m_close;                          //The close set
    std::vector<Edge> m_shortestPath;       //vector of edge in the shortest path

    unsigned int m_size;
    unsigned int m_densityPercentage;       //the density of edges
    unsigned int m_distanceRangeLowLimit;   //The low limit range distance
    unsigned int m_distanceRangeHighLimit;  //The high limit range distance

    inline unsigned int generateRandomNumber(unsigned int x_maxinum)const{
      unsigned int loc_result(0);
      loc_result = static_cast <unsigned int> (rand() % x_maxinum +1); //[0; x_maxinum]
      return loc_result;
    }

    /** \brief Random graph generator
     * \fn void generateGraph()
     * \return void
     *
     */
    void generateGraph();

    void init(node const& x_node);        //initialize open and close set for the computation of the shortest path
    void compute(node const& x_node);     //compute the shortest path from a given node.
};

#endif // SHORTESTPATH_H
