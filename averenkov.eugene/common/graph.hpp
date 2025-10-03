#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <array.hpp>
#include <hashTable.hpp>

namespace averenkov
{
  struct Graph
  {
    std::string name;
    HashTable< std::string, HashTable< std::string, Array< size_t > > > edges;
    HashTable< std::string, bool > vertices;
    void addEdge(const std::string& from, const std::string& to, size_t weight);
    void addVertex(const std::string& vertex);
  };

}

#endif

