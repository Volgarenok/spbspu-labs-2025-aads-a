#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include "hashtable.h"

namespace hismatova
{

  class GraphProcessor
  {
  private:
    HashTable< std::string, HashTable< std::string, HashTable< std::string, std::vector< int > > > > graphs;
    HashTable< std::string, HashTable< std::string, HashTable< std::string, std::vector< int > > > > inverse_graphs;
    HashTable< std::string, std::set< std::string > > vertices;

    std::vector< std::string > split(const std::string& str);
    bool graphExists(const std::string& graphName);
    bool vertexExists(const std::string& graphName, const std::string& vertex);
    void addVertex(const std::string& graphName, const std::string& vertex);
    void addEdge(const std::string& graphName, const std::string& from, const std::string& to, int weight);
    void removeEdge(const std::string& graphName, const std::string& from, const std::string& to, int weight);

    void executeGraphs(const std::vector< std::string >& tokens);
    void executeVertexes(const std::vector< std::string >& tokens);
    void executeOutbound(const std::vector< std::string >& tokens);
    void executeInbound(const std::vector< std::string >& tokens);
    void executeBind(const std::vector< std::string >& tokens);
    void executeCut(const std::vector< std::string >& tokens);
    void executeCreate(const std::vector< std::string >& tokens);
    void executeMerge(const std::vector< std::string >& tokens);
    void executeExtract(const std::vector< std::string >& tokens);

  public:
    void loadGraphsFromFile(const std::string& filename);
    void executeCommand(const std::string& commandLine);
  };

}

#endif
