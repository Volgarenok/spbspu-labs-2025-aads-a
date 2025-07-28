#ifndef DATASET_UTILS_HPP
#define DATASET_UTILS_HPP

#include <ios>
#include <string>
#include "tree.hpp"

namespace aleksandrov
{
  using Dataset = Tree< int, std::string >;
  using Datasets = Tree< std::string, Dataset >;

  void readDatasets(std::istream& in, Datasets& dest);
  void processCommand(const std::string& cmd, std::istream&, std::ostream&, Datasets& src);
  void printDataset(std::ostream& out, const std::string& name, const Datasets& src);
  void complement(const std::string& name, Dataset&, Dataset&, Datasets& src);
  void intersect(const std::string& name, Dataset&, Dataset&, Datasets& src);
  void unionCmd(const std::string& name, Dataset&, Dataset&, Datasets& src);
}

#endif

