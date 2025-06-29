#ifndef GRAPH_COMMANDS_HPP
#define GRAPH_COMMANDS_HPP
#include <functional>
#include "graph.hpp"

namespace alymova
{
  using GraphSet = HashTable< std::string, Graph, Hasher< std::string > >;
  using CommandSet = HashTable< std::string, std::function< void(GraphSet&) >, Hasher< std::string > >;

  struct GraphsCommand
  {
    std::ostream& out;
    void operator()(const GraphSet& graphs);
  };
  struct VertexesCommand
  {
    std::istream& in;
    std::ostream& out;
    void operator()(const GraphSet& graphs);
  };
  struct OutboundCommand
  {
    std::istream& in;
    std::ostream& out;
    void operator()(const GraphSet& graphs);
  };
  struct InboundCommand
  {
    std::istream& in;
    std::ostream& out;
    void operator()(const GraphSet& graphs);
  };
  struct BindCommand
  {
    std::istream& in;
    void operator()(GraphSet& graphs);
  };
  struct CutCommand
  {
    std::istream& in;
    void operator()(GraphSet& graphs);
  };
  struct CreateCommand
  {
    std::istream& in;
    void operator()(GraphSet& graphs);
  };
  struct MergeCommand
  {
    std::istream& in;
    void operator()(GraphSet& graphs);
  };
  struct ExtractCommand
  {
    std::istream& in;
    void operator()(GraphSet& graphs);
  };

  GraphSet readGraphsFile(std::istream& in);
  CommandSet complectCommands(std::istream& in, std::ostream& out);
}

#endif
