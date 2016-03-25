#ifndef CLOSE_H
#define CLOSE_H
#include <vector>
#include "Edge.h"

class Close
{
  public:
    //constructor
    Close(): m_edges({}){}

    //destructor
    virtual ~Close(){m_edges.clear();}

    //setters
    std::vector<Edge> getEges(){return m_edges;}

    /** \brief Check whether or not the edge exists in the close set
     * \fn bool Close::existEdge(Edge const& x_edge)const
     * \param [in] x_edge edge to check in
     * \return bool
     *
     */
    bool existEdge(Edge const& x_edge)const;

    /** \brief Add an edge to the close set
     * \fn void Close::addEdge(Edge const& x_edge)
     * \param [in] x_edge The edge to add to
     * \return void
     *
     */
    void addEdge(Edge const& x_edge);
  protected:
  private:
    std::vector<Edge> m_edges;    //vector of the close set
};

#endif // CLOSE_H
