#include "Graph.h"
#include "util.h"

void Graph::addNode(node const& x_node)
{
  if(!existNode(x_node)){
    m_nodes.push_back(x_node);
  }
}
void Graph::addEdge(Edge const& x_edge)
{
  if((existNode(x_edge.getSource())) && (existNode(x_edge.getDestination()))){
    if(!existEdge(x_edge)){
      m_edges.push_back(x_edge);
    }
  }
}
bool Graph::existNode(node const& x_node)
{
  return exist(m_nodes, x_node);
}
bool Graph::existEdge(Edge const& x_edge)
{
  return exist(m_edges, x_edge);
}
std::ostream& Graph::toOut(std::ostream& x_out) const
{
  x_out << std::endl << "nodes: {";
  x_out << m_nodes;
  x_out << "}" << std::endl;
  x_out << std::endl << "edges: {";
  x_out << m_edges;
  x_out << "}" << std::endl;
  return x_out;
}
std::vector<Edge> Graph::succesor(node const& x_node)
{
  std::vector<Edge> loc_edgeVector;
  nextNodes(m_edges, x_node, loc_edgeVector);
  return loc_edgeVector;
}

std::ostream& operator<<(std::ostream& x_out, Graph const& x_graph)
{
  return x_graph.toOut(x_out);
}
