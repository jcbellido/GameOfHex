#ifndef OPEN_H
#define OPEN_H
#include <vector>
#include "Edge.h"

class Open
{
  public:
    // constructor
    Open(): m_edges({}){}

    //destructor
    virtual ~Open(){m_edges.clear();}

    /** \brief Check whether or not an edge exits in the open set. The checking is based on the destination node
     * \fn bool Open::existEdge(Edge const& x_edge, Edge& x_edgeValue)const
     * \param [in] x_edge The edge of which the destination node is used for the search
     * \param [out] x_edgeValue The found edge
     * \return bool
     *
     */
    bool existEdge(Edge const& x_edge, Edge& x_edgeValue)const;

    /** \brief Add an edge to the open set
     * \fn void Open::addEdge(Edge const& x_edge)
     * \param [in] x_edge Edge to add to
     * \return void
     *
     */
    void addEdge(Edge const& x_edge);

    /** \brief check whether or not the open set is empty
     * \fn bool Open::isEmpty()const
     * \return bool
     *
     */
    bool isEmpty()const;

    /** \brief Return the edge with the least distance
     * \fn Edge Open::leastEdge()const
     * \return Edge
     *
     */
    Edge leastEdge()const;

    /** \brief Update the source and the distance of the giving edge in the open set
     * \fn void Open::updateEdge(Edge const& x_edge)
     * \param x_edge The edge to update
     * \return void
     *
     */
    void updateEdge(Edge const& x_edge);

    /** \brief Remove an edge from the open set
     * \fn void Open::removeEdge(Edge const& x_edge)
     * \param x_edge The edge to remove from the open set
     * \return void
     *
     */
    void removeEdge(Edge const& x_edge);
  protected:
  private:
    std::vector<Edge> m_edges;    // vector of the open set edges
};

#endif // OPEN_H
