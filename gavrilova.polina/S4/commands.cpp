#include "commands.hpp"
#include <stdexcept>

void gavrilova::printDataset(std::ostream& out, const std::string& datasetName, Dataset& datasets)
{
  auto it = datasets.find(datasetName);
  if (it == datasets.end()) {
    throw std::invalid_argument("Dataset not found");
  }

  const KeyMap& data = it->second;

  if (data.empty()) {
    out << "<EMPTY>\n";
    return;
  }

  out << datasetName;
  for (auto it = data.cbegin(); it != data.cend(); it++) {
    out << " " << it->first << " " << it->second;
  }
  out << "\n";
}

void gavrilova::complementDataset(const std::string& newDatasetName,
    const std::string& firstDatasetName,
    const std::string& secondDatasetName,
    Dataset& datasets)
{
  auto firstIt = datasets.find(firstDatasetName);
  auto secondIt = datasets.find(secondDatasetName);

  if (firstIt == datasets.end() || secondIt == datasets.end()) {
    throw std::invalid_argument("One or both datasets not found");
  }

  KeyMap newDataset;
  const auto& firstDataset = firstIt->second;
  const auto& secondDataset = secondIt->second;

  for (auto it = firstDataset.cbegin(); it != firstDataset.cend(); it++) {
    if (secondDataset.find(it->first) == secondDataset.end()) {
      newDataset.insert(*it);
    }
  }
  datasets[newDatasetName] = std::move(newDataset);
}

void gavrilova::intersectDatasets(const std::string& newDatasetName, const std::string& firstDatasetName,
    const std::string& secondDatasetName, Dataset& datasets)
{
  auto firstIt = datasets.find(firstDatasetName);
  auto secondIt = datasets.find(secondDatasetName);

  if (firstIt == datasets.end() || secondIt == datasets.end()) {
    throw std::invalid_argument("One or both datasets not found");
  }

  KeyMap newDataset;
  const auto& firstDataset = firstIt->second;
  const auto& secondDataset = secondIt->second;

  for (auto it = firstDataset.cbegin(); it != firstDataset.cend(); ++it) {
    if (secondDataset.find(it->first) != secondDataset.end()) {
      newDataset.insert(*it);
    }
  }

  datasets[newDatasetName] = std::move(newDataset);
}

void gavrilova::unionDatasets(const std::string& newDatasetName, const std::string& firstDatasetName,
    const std::string& secondDatasetName, Dataset& datasets)
{
  auto firstIt = datasets.find(firstDatasetName);
  auto secondIt = datasets.find(secondDatasetName);

  if (firstIt == datasets.end() || secondIt == datasets.end()) {
    throw std::invalid_argument("One or both datasets not found");
  }

  KeyMap newDataset;
  const auto& firstDataset = firstIt->second;
  const auto& secondDataset = secondIt->second;

  for (const auto& dataset: {firstDataset, secondDataset}) {
    for (auto it = dataset.cbegin(); it != dataset.cend(); it++) {
      newDataset.insert(std::make_pair(it->first, it->second));
    }
  }
  datasets[newDatasetName] = std::move(newDataset);
}
