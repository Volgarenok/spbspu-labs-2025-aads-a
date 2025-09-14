#ifndef DICT_UTILS_HPP
#define DICT_UTILS_HPP
#include <iostream>
#include <functional>
#include <tree/tree.hpp>

namespace sveshnikov
{
  using Dict_t = sveshnikov::AvlTree< int, std::string >;
  using DataTree_t = sveshnikov::AvlTree< std::string, Dict_t >;
  using CommandHolder_t =
      sveshnikov::AvlTree< std::string, std::function< void(std::istream &, DataTree_t &) > >;

  DataTree_t loadDicts(char *filename);
  CommandHolder_t getCommands();

  void printDict(std::istream &in, const DataTree_t &data);
  void complementDict(std::istream &in, DataTree_t &data);
  void intersectDict(std::istream &in, DataTree_t &data);
  void unionDict(std::istream &in, DataTree_t &data);
}
#endif
