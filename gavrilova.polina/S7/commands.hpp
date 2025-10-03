#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "HashTable.hpp"
#include "WeightedGraph.hpp"

namespace gavrilova {

  using Graph = WeightedGraph< std::string, unsigned int >;
  using GraphsCollection = HashTable< std::string, Graph >;

  void graphs(std::ostream& out, const GraphsCollection& graphs);
  void vertexes(std::ostream& out, std::istream& in, const GraphsCollection& graphs);
  void outbound(std::ostream& out, std::istream& in, const GraphsCollection& graphs);
  void inbound(std::ostream& out, std::istream& in, const GraphsCollection& graphs);
  void bind(std::istream& in, GraphsCollection& graphs);
  void cut(std::istream& in, GraphsCollection& graphs);
  void create(std::istream& in, GraphsCollection& graphs);
  void merge(std::istream& in, GraphsCollection& graphs);
  void extract(std::istream& in, GraphsCollection& graphs);

  void readGraphsFromFile(std::istream& in, GraphsCollection& graphs);

}

#endif
