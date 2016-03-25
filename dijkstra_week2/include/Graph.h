#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include "Edge.h"

class Graph
{
  public:
    //constructor
    Graph():m_nodes({}), m_edges({}){}

    //destructor
    virtual ~Graph(){m_nodes.clear(); m_edges.clear();}

    /** \brief Add a node to a Graph
     * fn void Graph::addNode(node const& x_node)
     * \param [in] x_node The node to add
     * \return void
     *
     */
    void addNode(node const& x_node);

    /** \brief Add a node to a Graph
     * \fn void Graph::addEdge(Edge const& x_edge)
     * \param [in] x_edge edge to add
     * \return void
     *
     */
    void addEdge(Edge const& x_edge);

    /** \brief check whether or not an node exists in the graph
     * \fn bool Graph::existNode(node const& x_node)
     * \param [in] x_node The node to looking for
     * \return bool
     *
     */
    bool existNode(node const& x_node);

    /** \brief check whether or not an edge with the same source and the same destination exists in the graph
     * \fn bool Graph::existEdge(Edge const& x_edge)
     * \param [in] x_edge The edge to looking for
     * \return bool
     *
     */
    bool existEdge(Edge const& x_edge);

    /** \brief Return a vector of edges where the source is the node x_node
     * \fn std::vector<Edge> Graph::succesor(node const& x_node)
     * \param [in] x_node The node of which the successors are looking for
     * \return std::vector<Edge>
     *
     */
    std::vector<Edge> succesor(node const& x_node);

    /** \brief Output the graph as a stream
     * \fn std::ostream& Graph::toOut(std::ostream& x_out)
     * \param [in, out] x_out The stream
     * \return std::ostream&
     *
     */
    std::ostream& toOut(std::ostream& x_out)const;

  protected:
  private:
    std::vector<node> m_nodes;    //vector of the graph nodes
    std::vector<Edge> m_edges;    //vector of the graph edges
};

/** \brief Overloaded operator<< for Graph
 * \fn std::ostream& operator<<(std::ostream& x_out, Graph const& x_graph)
 * \param [in, out] x_out The output stream
 * \param x_graph The graph to outpout
 * \return std::ostream&
 *
 */
std::ostream& operator<<(std::ostream& x_out, Graph const& x_graph);

#endif // GRAPH_H
