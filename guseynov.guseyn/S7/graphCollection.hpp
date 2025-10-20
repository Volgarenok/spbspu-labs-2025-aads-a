#ifndef GRAPHCOLLECTION_HPP
#define GRAPHCOLLECTION_HPP
#include "hashTable.hpp"
#include "graph.hpp"

namespace guseynov
{
  class GraphCollection
  {
  public:
    bool addGraph(const std::string & name);
    Graph * getGraph(const std::string & name);
    const Graph * getGraph(const std::string & name) const;
    std::vector< std::string > listGraphs() const;
  private:
    HashTable< std::string, Graph > graphs_;
  };
}
#endif
