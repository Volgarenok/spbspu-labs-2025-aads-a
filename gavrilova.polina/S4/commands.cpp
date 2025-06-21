#include "commands.hpp"
#include <stdexcept>

namespace gavrilova {

  void printDataset(std::ostream& out, const std::string& datasetName, Dataset& datasets)
  {
    try {
      const KeyMap& data = datasets.at(datasetName);

      if (data.empty()) {
        out << "<EMPTY>\n";
        return;
      }

      out << datasetName;
      for (auto it = data.cbegin(); it != data.cend(); it++) {
        out << " " << it->first << " " << it->second;
      }
      out << "\n";
    } catch (const std::out_of_range&) {
      throw std::invalid_argument("");
    }
  }

  void complementDataset(const std::string& newDatasetName,
      const std::string& firstDatasetName,
      const std::string& secondDatasetName,
      Dataset& datasets)
  {
    try {
      const auto& firstDataset = datasets.at(firstDatasetName);
      const auto& secondDataset = datasets.at(secondDatasetName);

      KeyMap newDataset;

      for (auto it = firstDataset.cbegin(); it != firstDataset.cend(); it++) {
        if (secondDataset.find(it->first) == secondDataset.end()) {
          newDataset.insert(*it);
        }
      }
      datasets[newDatasetName] = std::move(newDataset);
    } catch (const std::out_of_range&) {
      throw std::invalid_argument("");
    }
  }

  void intersectDatasets(const std::string& newDatasetName, const std::string& firstDatasetName,
      const std::string& secondDatasetName, Dataset& datasets)
  {
    try {
      const auto& firstDataset = datasets.at(firstDatasetName);
      const auto& secondDataset = datasets.at(secondDatasetName);

      KeyMap newDataset;

      for (auto it = firstDataset.cbegin(); it != firstDataset.cend(); it++) {
        if (secondDataset.find(it->first) != secondDataset.end()) {
          newDataset.insert(*it);
        }
      }
      datasets[newDatasetName] = std::move(newDataset);
    } catch (const std::out_of_range&) {
      throw std::invalid_argument("");
    }
  }

  void unionDatasets(const std::string& newDatasetName, const std::string& firstDatasetName,
      const std::string& secondDatasetName, Dataset& datasets)
  {
    try {
      const auto& firstDataset = datasets.at(firstDatasetName);
      const auto& secondDataset = datasets.at(secondDatasetName);

      KeyMap newDataset;

      for (const auto& dataset: {firstDataset, secondDataset}) {
        for (auto it = dataset.cbegin(); it != dataset.cend(); it++) {
          newDataset.insert(std::make_pair(it->first, it->second));
        }
      }
      datasets[newDatasetName] = std::move(newDataset);
    } catch (const std::out_of_range&) {
      throw std::invalid_argument("");
    }
  }

}
