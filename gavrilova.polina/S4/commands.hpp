#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <tree/ConstIterator.hpp>
#include <tree/Iterator.hpp>
#include <tree/TwoThreeTree.hpp>

namespace gavrilova {
  using KeyMap = TwoThreeTree< size_t, std::string >;
  using Dataset = TwoThreeTree< std::string, KeyMap >;

  void printDataset(std::ostream& out, const std::string& datasetName, Dataset& datasets);

  void complementDataset(const std::string& newDatasetName, const std::string& firstDatasetName,
      const std::string& secondDatasetName, Dataset& datasets);

  void intersectDatasets(const std::string& newDatasetName, const std::string& firstDatasetName,
      const std::string& secondDatasetName, Dataset& datasets);

  void unionDatasets(const std::string& newDatasetName, const std::string& firstDatasetName,
      const std::string& secondDatasetName, Dataset& datasets);
}

#endif
