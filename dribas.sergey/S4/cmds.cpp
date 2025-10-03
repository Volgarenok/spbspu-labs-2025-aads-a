#include "cmds.hpp"

void dribas::print(std::ostream& out , std::istream& input, const dataset& setdata)
{
  std::string name;
  input >> name;
  data tree = setdata.at(name);
  if (tree.empty()) {
    out << "<EMPTY>\n";
    return;
  }
  out << name;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    out << " " << it->first << " " << it->second;
  }
  out << "\n";
}

void dribas::complement(std::istream& input, dataset& setOfData)
{
  std::string newName, firstName, secondName;
  input >> newName >> firstName >> secondName;
  data newTree;
  auto firstTree = setOfData.at(firstName);
  auto secondTree = setOfData.at(secondName);
  for (auto it = firstTree.begin(); it != firstTree.end(); ++it) {
    if (secondTree.find(it->first) == secondTree.end()) {
      newTree.insert(*it);
    }
  }
  setOfData[newName] = newTree;
}

void dribas::intersect(std::istream& input, dataset& setOfData)
{
  std::string newName, firstName, secondName;
  input >> newName >> firstName >> secondName;
  data newTree;
  auto firstTree = setOfData.at(firstName);
  auto secondTree = setOfData.at(secondName);
  for (auto it = firstTree.begin(); it != firstTree.end(); ++it) {
    if (secondTree.find(it->first) != secondTree.end()) {
      newTree.insert(*it);
    }
  }
  setOfData[newName] = newTree;
}

void dribas::unite(std::istream& input, dataset& setOfData)
{
  std::string newName, firstName, secondName;
  input >> newName >> firstName >> secondName;
  auto firstTree = setOfData.at(firstName);
  auto secondTree = setOfData.at(secondName);
  data newTree = firstTree;
  for (auto it = secondTree.begin(); it != secondTree.end(); ++it)
  {
    newTree.insert(*it);
  }
  setOfData[newName] = newTree;
}
