#ifndef DATASETCOMMANDS_HPP
#define DATASETCOMMANDS_HPP
#include <map>
#include <string>
#include <fstream>
#include <binary_tree/binary_tree.hpp>

namespace abramov
{
  using tree = BinarySearchTree< int, std::string >;
  using collections = BinarySearchTree< std::string, tree >;
  collections getDataSets(std::ifstream &in);
  void printDataSet(const std::string &s, const collections &dicts);
  void complementDataSets(const std::string &s, const std::string &s1, const std::string &s2, collections &dicts);
  void intersectDataSets(const std::string &s, const std::string &s1, const std::string &s2, collections &dicts);
  void unionDataSets(const std::string &s, const std::string &s1, const std::string &s2, collections &dicts);
  void doCommand(const std::string &s, std::istream &in, collections &dicts);
}
#endif
