#include "Open.h"

bool Open::existEdge(Edge const& x_edge, Edge& x_edgeValue)const
{
  return existAsDestination(m_edges, x_edge.getDestination(), x_edgeValue);
}
void Open::addEdge(Edge const& x_edge)
{
  Edge loc_edge = x_edge;
  if(!existEdge(x_edge, loc_edge)){
    m_edges.push_back(x_edge);
  }
}
bool Open::isEmpty()const
{
  return m_edges.empty();
}
Edge Open::leastEdge() const
{
  return smallest(m_edges);
}
void Open::updateEdge(Edge const& x_edge)
{
  update(m_edges, x_edge);
}
void Open::removeEdge(Edge const& x_edge)
{
  removeEdgeByDestination(m_edges, x_edge.getDestination());
}
