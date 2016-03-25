#ifndef EDGE_H
#define EDGE_H
#include <iostream>
#include "util.h"

class Edge
{
  public:
    //constructor
    Edge(node const& x_source, node const& x_destination, distance const& x_cost): m_source(x_source), m_destination(x_destination), m_cost(x_cost) {}
    Edge(Edge const& x_edge): m_source(x_edge.m_source), m_destination(x_edge.m_destination), m_cost(x_edge.m_cost) {}

    //destructor
    virtual ~Edge(){}

    //getters
    node getSource() const { return m_source; }
    node getDestination() const { return m_destination; }
    distance getCost() const { return m_cost; }

    //setters
    void setSource(node x_source) { m_source = x_source; }
    void setDestination(node x_destination) { m_destination = x_destination; }
    void setCost(distance x_cost) { m_cost = x_cost; }

    const Edge invert ()const;                        //invert source and destination
    std::ostream& toOut(std::ostream& x_out) const;   //output stream
    Edge& operator=(Edge const& x_edge);              //Overloaded operator=
  protected:
  private:
    node m_source;        //The source node of the edge
    node m_destination;   //The destination node of the edge
    distance m_cost;      //The distance of the edge
};

/** \brief Overloaded operator<< for Edge. Output an edge according to the format (sourceNode -< destinationNode: distance)
 * \fn std::ostream& operator<<(std::ostream& x_out, Edge const& x_edge)
 * \param [in, out] x_out The output stream
 * \param [in] x_edge The edge to output
 * \return std::ostream&
 *
 */
std::ostream& operator<<(std::ostream& x_out, Edge const& x_edge);

/** \brief Overloaded operator== for Edge. Two adges are the same if they have same source and same destination
 * \fn bool operator==(Edge const& x_edge1, Edge const& x_edge2)
 * \param [in] x_edge1 The first edge
 * \param [in] x_edge2 The second edge
 * \return bool
 *
 */
bool operator==(Edge const& x_edge1, Edge const& x_edge2);

/** \brief Overloaded operator< for Edge. The comparison is based on the cost. return true is x_edge1 has less cost than x_edge2
 * \fn bool operator<(Edge const& x_edge1, Edge const& x_edge2)
 * \param [in] x_edge1 The first edge
 * \param [in] x_edge2 The second edge
 * \return bool
 *
 */
bool operator<(Edge const& x_edge1, Edge const& x_edge2);

void nextNodes(std::vector<Edge> const& x_vector, node const& x_node, std::vector<Edge>& x_successor);

/** \brief Looking whether or not there exist an edge x_edge with x_node as destination of this edge in the edge vector x_vector
 * \fn bool existAsDestination(std::vector<Edge> const& x_vector, node const& x_node, Edge& x_edge)
 * \param [in] x_vector The vector to looking in
 * \param [in] x_node The source node to search for
 * \param [out] x_edge The found edge
 * \return bool
 *
 */
bool existAsDestination(std::vector<Edge> const& x_vector, node const& x_node, Edge& x_edge);

/** \brief Looking whether or not there exist an edge x_edge with x_sourceNode as source of this edge in the edge vector x_vector
 * \fn bool existAsSource(std::vector<Edge> const& x_vector, node const& x_sourceNode, Edge& x_edge)
 * \param [in] x_vector The vector to looking in
 * \param [in] x_sourceNode The source node to search for
 * \param [out] x_edge The found edge
 * \return bool
 *
 */
bool existAsSource(std::vector<Edge> const& x_vector, node const& x_sourceNode, Edge& x_edge);

/** \brief Update the source and the distance of an edge in a given vector. The destination node of the edge is used as the index to find the edge to update in the vector.
 * \fn void update(std::vector<Edge>& x_vector, Edge const& x_edge)
 * \param [in, out] x_vector The vector containing the edge to update
 * \param [in] x_edge The edge to update
 * \return void
 *
 */
void update(std::vector<Edge>& x_vector, Edge const& x_edge);

/** \brief Remove an edge from a vector edge given a node considered as a destination of the edge to remove
 * \fn void removeEdgeByDestination(std::vector<Edge>& x_vector, node const& x_node)
 * \param [in, out] x_vector The vector to remove from
 * \param [in] x_node The destination node of the edge to remove
 * \return void
 *
 */
void removeEdgeByDestination(std::vector<Edge>& x_vector, node const& x_node);

/** \brief Output the content of an edge vector with the format (edge_1-edge_2- ... -edge_n)
 * \fn std::ostream& printPath(std::ostream& x_out, std::vector<Edge> const& x_vector)
 * \param [in, out] x_out The output stream
 * \param [in] x_vector The vector of elements to output
 * \return std::ostream&
 *
 */
std::ostream& printPath(std::ostream& x_out, std::vector<Edge> const& x_vector);

/** \brief Given an edge vector x_vector the function compute the short path from node x_source to node x_destination. the edge of the path is the store in x_path and the cost of this path is store in x_distance. The return value indicate if the path exists
 * \fn bool path(std::vector<Edge>const& x_vector, node const& x_source, node x_destination, std::vector<Edge>& x_path, distance& x_distance)
 * \param [in] x_vector The vector of edge where the shortest path is find
 * \param [in] x_source The source node of the shorted path
 * \param [in] x_destination the destination node of the shorted path
 * \param [out] x_path the vector of edges that constitute the shortest path
 * \param [out] x_distance The distance of the shortest path
 * \return bool
 *
 */
bool path(std::vector<Edge>const& x_vector, node const& x_source, node x_destination, std::vector<Edge>& x_path, distance& x_distance);
#endif // EDGE_H
