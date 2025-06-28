#include "tree_actions.hpp"

#include <iostream>

void zholobov::doPrint(std::ostream& out, std::istream& in, const NamedTrees& namedTrees)
{
  std::string datasetName;
  in >> datasetName;
  const TreeMap& map = namedTrees.at(datasetName);
  if (map.empty()) {
    out << "<EMPTY>\n";
    return;
  }

  out << datasetName;
  for (const auto& key: map) {
    out << " " << key.first << " " << key.second;
  }
  out << "\n";
}

void zholobov::doComplement(std::istream& in, NamedTrees& namedTrees)
{
  std::string newName, lhsName, rhsName;
  in >> newName >> lhsName >> rhsName;
  const TreeMap& lhs = namedTrees.at(lhsName);
  const TreeMap& rhs = namedTrees.at(rhsName);
  TreeMap newMap;
  if (lhsName != rhsName) {
    for (const auto& elem: lhs) {
      if (rhs.find(elem.first) == rhs.end()) {
        newMap.insert(elem);
      }
    }
  }
  if (newName == lhsName) {
    namedTrees[lhsName] = std::move(newMap);
  } else if (newName == rhsName) {
    namedTrees[rhsName] = std::move(newMap);
  } else {
    namedTrees[newName] = std::move(newMap);
  }
}

void zholobov::doIntersect(std::istream& in, NamedTrees& namedTrees)
{
  std::string newName, lhsName, rhsName;
  in >> newName >> lhsName >> rhsName;
  const TreeMap& lhs = namedTrees.at(lhsName);
  const TreeMap& rhs = namedTrees.at(rhsName);
  if ((lhsName == rhsName) && (newName != lhsName)) {
    namedTrees[newName] = lhs;
  } else {
    TreeMap newMap;
    for (const auto& elem: lhs) {
      if (rhs.find(elem.first) != rhs.end()) {
        newMap.insert(elem);
      }
    }
    if (newName == lhsName) {
      namedTrees[lhsName] = std::move(newMap);
    } else if (newName == rhsName) {
      namedTrees[rhsName] = std::move(newMap);
    } else {
      namedTrees[newName] = std::move(newMap);
    }
  }
}

void zholobov::doUnion(std::istream& in, NamedTrees& namedTrees)
{
  std::string newName, lhsName, rhsName;
  in >> newName >> lhsName >> rhsName;
  const TreeMap& lhs = namedTrees.at(lhsName);
  const TreeMap& rhs = namedTrees.at(rhsName);
  if ((lhsName == rhsName) && (newName != lhsName)) {
    namedTrees[newName] = lhs;
  } else {
    TreeMap newMap = lhs;
    for (const auto& elem: rhs) {
      if (newMap.find(elem.first) == newMap.end()) {
        newMap.insert(elem);
      }
    }

    if (newName == lhsName) {
      namedTrees[lhsName] = std::move(newMap);
    } else if (newName == rhsName) {
      namedTrees[rhsName] = std::move(newMap);
    } else {
      namedTrees[newName] = std::move(newMap);
    }
  }
}
