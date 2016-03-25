#include "Close.h"

bool Close::existEdge(Edge const& x_edge)const
{
  Edge loc_tempEdge = x_edge;
  return existAsDestination(m_edges, x_edge.getDestination(), loc_tempEdge);
}
void Close::addEdge(Edge const& x_edge)
{
  if(!existEdge(x_edge)){
    m_edges.push_back(x_edge);
  }
}
