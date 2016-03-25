#include "Edge.h"

const Edge Edge::invert ()const
{
  Edge loc_edge(m_destination, m_source, m_cost);
  return loc_edge;
}
std::ostream& Edge::toOut(std::ostream& x_out) const
{
  x_out << "(" << m_source << " -> " << m_destination << ": " << m_cost << ")";
  return x_out;
}
Edge& Edge::operator=(Edge const& x_edge)
{
  m_source = x_edge.m_source;
  m_destination = x_edge.m_destination;
  m_cost = x_edge.m_cost;
  return *this;
}

std::ostream& operator<<(std::ostream& x_out, Edge const& x_edge)
{
  return x_edge.toOut(x_out);
}
bool operator==(Edge const& x_edge1, Edge const& x_edge2)
{
  return (x_edge1.getSource() == x_edge2.getSource()) && (x_edge1.getDestination() == x_edge2.getDestination());
}
bool operator<(Edge const& x_edge1, Edge const& x_edge2)
{
  return (x_edge1.getCost() < x_edge2.getCost());
}
void nextNodes(std::vector<Edge> const& x_vector, node const& x_node, std::vector<Edge>& x_successor)
{
  for(auto loc_edge: x_vector){
      if(x_node == loc_edge.getSource()){
        x_successor.push_back(loc_edge);
      }
  }
}
bool existAsDestination(std::vector<Edge> const& x_vector, node const& x_destinationNode, Edge& x_edge)
{
  bool loc_result(false);
  for(auto loc_edge: x_vector){
    if(loc_edge.getDestination() == x_destinationNode){
      loc_result = true;
      x_edge = loc_edge;
      break;
    }
  }
  return loc_result;
}
bool existAsSource(std::vector<Edge> const& x_vector, node const& x_sourceNode, Edge& x_edge)
{
  bool loc_result(false);
  for(auto loc_edge: x_vector){
    if(loc_edge.getSource() == x_sourceNode){
      loc_result = true;
      x_edge = loc_edge;
      break;
    }
  }
  return loc_result;
}
void update(std::vector<Edge>& x_vector, Edge const& x_edge)
{
  for(auto& loc_edge: x_vector){
    if(loc_edge.getDestination() == x_edge.getDestination()){
      loc_edge.setSource(x_edge.getSource());
      loc_edge.setCost(x_edge.getCost());
      break;
    }
  }
}
void removeEdgeByDestination(std::vector<Edge>& x_vector, node const& x_destinationNode)
{
  for(auto& loc_edge: x_vector){
    if(loc_edge.getDestination() == x_destinationNode){
      std::vector<Edge>::iterator loc_it = x_vector.end();
      --loc_it;
      Edge loc_tempEdge = *loc_it;
      *loc_it = loc_edge;
      loc_edge = loc_tempEdge;
      x_vector.pop_back();
      break;
    }
  }
}
std::ostream& printPath(std::ostream& x_out, std::vector<Edge> const& x_vector)
{
  bool loc_isFirst(true);
  std::vector<Edge>::const_reverse_iterator it = x_vector.rbegin();
  for(; it != x_vector.rend(); ++it){
    if(loc_isFirst){
      x_out << (*it).getDestination();
    }else{
      x_out << "-" << (*it).getDestination();
    }
    loc_isFirst = false;
  }
  return x_out;
}
bool path(std::vector<Edge>const& x_vector, node const& x_source, node x_destination, std::vector<Edge>& x_path, distance& x_distance)
{
  bool loc_find(false);
  x_distance = 0;
  Edge loc_edge(0, 0, 0);
  while((existAsDestination(x_vector, x_destination, loc_edge)) && (!loc_find)){
    x_path.push_back(loc_edge);
    x_distance += loc_edge.getCost();
    x_destination = loc_edge.getSource();
    loc_find = (x_source == x_destination) && (x_source == loc_edge.getDestination());
  }
  if(!loc_find){
    x_path.clear();
    x_distance = 0;
  }
  return loc_find;
}
