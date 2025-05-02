#ifndef DATASETCOMMANDS_HPP
#define DATASETCOMMANDS_HPP
#include <map>
#include <string>
#include <fstream>

namespace abramov
{
  using collection = std::map< std::string, std::map< int, std::string > >;
  std::map< std::string, std::map< int, std::string > > getDataSets(std::ifstream &in);
  void printDataSet(const std::string &s, const collection &dicts);
  std::map< int, std::string > complementDataSets(const std::string &s1, const std::string &s2, const collection &dicts);
  std::map< int, std::string > intersectDataSets(const std::string &s1, const std::string &s2, const collection &dicts);
  std::map< int, std::string > unionDataSets(const std::string &s1, const std::string &s2, const collection &dicts);
  void doCommand(const std::string &s, std::istream &in, collection &dicts);
}
#endif
