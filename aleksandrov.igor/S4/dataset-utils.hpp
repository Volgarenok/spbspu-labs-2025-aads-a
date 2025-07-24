#ifndef DATASET_UTILS_HPP
#define DATASET_UTILS_HPP

#include "tree.hpp"
#include <string>
#include <iostream>

namespace aleksandrov
{
  using Dataset = Tree< int, std::string >;
  using DatasetCollection = Tree< std::string, Dataset >;
  using str = const std::string&;

  DatasetCollection readDatasets(std::istream&);
  void processCommand(str, std::istream& in, DatasetCollection&);
  void printDataset(str, const DatasetCollection&);
  void complementDataset(str, str, str, DatasetCollection&);
  void intersectDataset(str, str, str, DatasetCollection&);
  void unionDataset(str, str, str, DatasetCollection&);
}

#endif

