#include "commandList.hpp"
#include <stdexcept>
#include <utility>

void brevnov::print(std::ostream& out, std::istream& in, const dataset& dict)
{
  std::string name;
  in >> name;
  data tree = dict.at(name);
  if (tree.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  out << name;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    out << " " << it->first << " " << it->second;
  }
  out << "\n";
}

void brevnov::complement(std::istream& in, dataset& dict)
{
  std::string newName;
  std::string firstName;
  std::string secondName;
  in >> newName >> firstName >> secondName;
  data newTree;
  data firstTree = dict.at(firstName);
  data secondTree = dict.at(secondName);
  for (auto it = firstTree.cbegin(); it != firstTree.cend(); ++it)
  {
    if (secondTree.find(it->first) == secondTree.end())
    {
      newTree.insert(*it);
    }
  }
  dict[newName] = newTree;
}

void brevnov::intersect(std::istream& in, dataset& dict)
{
  std::string newName;
  std::string firstName;
  std::string secondName;
  in >> newName >> firstName >> secondName;
  data newTree;
  data firstTree = dict.at(firstName);
  data secondTree = dict.at(secondName);
  for (auto it = firstTree.cbegin(); it != firstTree.cend(); ++it)
  {
    if (secondTree.find(it->first) != secondTree.end())
    {
      newTree.insert(*it);
    }
  }
  dict[newName] = newTree;
}

void brevnov::unite(std::istream& in, dataset& dict)
{
  std::string newName;
  std::string firstName;
  std::string secondName;
  in >> newName >> firstName >> secondName;
  data firstTree = dict.at(firstName);
  data secondTree = dict.at(secondName);
  data newTree = firstTree;
  for (auto it = secondTree.cbegin(); it != secondTree.cend(); ++it)
  {
    newTree.insert(*it);
  }
  dict[newName] = newTree;
}
