#ifndef DATASET_UTILS_HPP
#define DATASET_UTILS_HPP

#include <map>
#include <string>
#include <iostream>

namespace aleksandrov
{
  using Dataset = std::map< int, std::string >;
  using DatasetCollection = std::map< std::string, Dataset >;
  using str = const std::string&;

  DatasetCollection readDatasets(std::istream&);
  void processCommand(str, std::istream& in, DatasetCollection&);
  void printDataset(str, const DatasetCollection&);
  void complementDataset(str, str, str, DatasetCollection&);
  void intersectDataset(str, str, str, DatasetCollection&);
  void unionDataset(str, str, str, DatasetCollection&);
}

#endif

