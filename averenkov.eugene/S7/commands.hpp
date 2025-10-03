#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <BiTree.hpp>
#include <graph.hpp>

namespace averenkov
{
  void loadGraphsFromFile(Tree< std::string, Graph >& graphs, std::istream& in);
  void printGraphs(std::ostream& out, const Tree< std::string, Graph >& graphs);
  void printVertices(std::ostream& out, std::istream& in, const Tree< std::string, Graph >& graphs);
  void printOutbound(std::ostream& out, std::istream& in, const Tree< std::string, Graph >& graphs);
  void printInbound(std::ostream& out, std::istream& in, const Tree< std::string, Graph >& graphs);
  void bindEdge(std::istream& in, Tree< std::string, Graph >& graphs);
  void cutEdge(std::istream& in, Tree< std::string, Graph >& graphs);
  void createGraph(std::istream& in, Tree< std::string, Graph >& graphs);
  void mergeGraphs(std::istream& in, Tree< std::string, Graph >& graphs);
  void extractGraph(std::istream& in, Tree< std::string, Graph >& graphs);
  void commandsInit(Tree< std::string, std::function< void() > >& cmds, Tree< std::string, Graph >& graphs);
}

#endif
