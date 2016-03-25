#include "ShortestPath.h"

void ShortestPath::init(node const& x_node)
{
  Edge loc_sourceEdge(x_node, x_node, 0);
  m_close.addEdge(loc_sourceEdge);
  std::vector<Edge> loc_sucessorVector = m_graph.succesor(x_node);
  for(auto loc_sucessor: loc_sucessorVector){
    m_open.addEdge(loc_sucessor);
  }
}
void ShortestPath::compute(node const& x_node)
{
  init(x_node);
  Edge loc_edge(0, 0, 0), loc_currentEdge(0, 0, 0);
  std::vector<Edge> loc_sucessorVector({});
  while(!m_open.isEmpty()){
    loc_edge = m_open.leastEdge();
    m_close.addEdge(loc_edge);
    loc_sucessorVector = m_graph.succesor(loc_edge.getDestination());
    for(auto loc_sucessor: loc_sucessorVector){
        if(!(m_open.existEdge(loc_sucessor, loc_currentEdge)) && !(m_close.existEdge(loc_sucessor))){
          m_open.addEdge(loc_sucessor);
        }else if(m_open.existEdge(loc_sucessor, loc_currentEdge)){
          distance loc_distance = loc_edge.getCost() + loc_sucessor.getCost();
          if(loc_distance < loc_currentEdge.getCost()){
            loc_currentEdge = loc_sucessor;
            loc_currentEdge.setCost(loc_distance);
            m_open.updateEdge(loc_currentEdge);
          }
        }
    }
    m_open.removeEdge(loc_edge);
  }
}

bool ShortestPath::path_size(node const& x_source, node const& x_destination, distance& x_cost)
{
  bool loc_result(false);
  m_shortestPath.clear();
  loc_result = path(m_close.getEges(),  x_source, x_destination, m_shortestPath, x_cost);
  printPath(std::cout, m_shortestPath);
  return loc_result;
}

void ShortestPath::generateGraph()
{
  srand(clock());
  distance loc_cost(0);
  Edge loc_edge(0, 0, 0);
  for(node loc_node(0); loc_node < m_size; ++loc_node){
    m_graph.addNode(loc_node);
  }
  for(node loc_source(0); loc_source < m_size; ++loc_source){
    for(unsigned int loc_destination(loc_source + 1); loc_destination < m_size; ++loc_destination){
        if(generateRandomNumber(100) <= m_densityPercentage){
            loc_cost = m_distanceRangeLowLimit + (m_distanceRangeHighLimit - m_distanceRangeLowLimit) * (generateRandomNumber(100) + 1) / 100;
            loc_edge.setSource(loc_source);
            loc_edge.setDestination(loc_destination);
            loc_edge.setCost(loc_cost);
            m_graph.addEdge(loc_edge);
            m_graph.addEdge(loc_edge.invert());
        }
    }
  }
  std::cout << std::endl << m_graph << std::endl;
}

void ShortestPath::printAllShortestPathFromNode(node x_source)
{
  double loc_average(0);
  unsigned int loc_nomberOfPath(0);
  distance loc_cost(0);
  compute(x_source);
  for(unsigned int loc_destination(0); loc_destination < m_size; ++loc_destination){
      if(x_source != loc_destination){
      std::cout << std::endl << "Shortest path from " << x_source << " to " << loc_destination << " : " << std::endl;
      if(path_size(x_source, loc_destination, loc_cost)){
        std::cout << std::endl << "Distance : " << loc_cost << std::endl;
        loc_average += loc_cost;
        ++loc_nomberOfPath;
      }
      else{
        std::cout << std::endl << "does not exist" << std::endl;
      }
    }
  }
  if(loc_nomberOfPath > 0){
    loc_average /= static_cast<double>(loc_nomberOfPath);
    std::cout << std::endl << "Average distance : " << loc_average << std::endl;
  }
}
