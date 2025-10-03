#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <utility>
#include <set>
#include "robinHashTable.hpp"

namespace dribas
{
  class Graph
  {
  public:
    Graph() = default;
    Graph(const Graph&, const RobinHoodHashTable< std::string, bool >&);
    Graph(const Graph&, const Graph&);

    void addVertex(const std::string&);
    bool addEdge(const std::string&, const std::string&, int);
    bool removeEdge(const std::string&, const std::string&, int);

    bool hasVertex(const std::string&) const;

    std::vector< std::pair< std::string, std::vector< int > > > getOutboundEdges(const std::string&) const;
    std::vector< std::pair< std::string, std::vector< int > > > getInboundEdges(const std::string&x) const;

    std::vector<std::string> getUniqueVertexes() const;

    friend std::istream& operator>>(std::istream&, Graph&);
    friend std::ostream& operator<<(std::ostream&, const Graph&);

  private:
    RobinHoodHashTable< std::pair< std::string, std::string >, std::vector< int > > faces_;
    std::set< std::string > vertices_;
  };

  std::istream& operator>>(std::istream&, Graph&);
  std::ostream& operator<<(std::ostream&, const Graph&);
}

#endif
