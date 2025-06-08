#ifndef DATASET_H
#define DATASET_H
#include <map>
#include <string>
#include <functional>
#include "two-three-tree.h"

using Dataset = savintsev::TwoThreeTree< int, std::string >;
using DatasetCollection = savintsev::TwoThreeTree< std::string, Dataset >;
using DatasetCommands = savintsev::TwoThreeTree< std::string, std::function< void(DatasetCollection &) > >;

namespace savintsev
{
  DatasetCollection load_dataset_from(const std::string & filename);

  void print_dataset(const DatasetCollection & datasets);
  void complement_datasets(DatasetCollection & datasets);
  void intersect_datasets(DatasetCollection & datasets);
  void union_datasets(DatasetCollection & datasets);
}

#endif
