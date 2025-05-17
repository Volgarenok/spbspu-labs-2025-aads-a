#include "tree_manips.hpp"

void demehin::print(std::ostream& out, std::istream& in, const MapOfTrees& mapOfTrees)
{
  std::string datasetName;
  in >> datasetName;
  TreeMap map = mapOfTrees.at(datasetName);
  if (map.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  out << datasetName;
  for (auto&& key: map)
  {
    out << " " << key.first << " " << key.second;
  }
  out << "\n";
}

void demehin::makeComplement(std::istream& in, MapOfTrees& mapOfTrees)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  TreeMap newMap, map1, map2;
  if (mapOfTrees.at(name1).size() > mapOfTrees.at(name2).size())
  {
    map1 = mapOfTrees.at(name1);
    map2 = mapOfTrees.at(name2);
  }
  else
  {
    map1 = mapOfTrees.at(name2);
    map2 = mapOfTrees.at(name1);
  }

  for (auto&& key: map1)
  {
    if (map2.find(key.first) == map2.end())
    {
      newMap.insert(key);
    }
  }

  if (newName == name1)
  {
    mapOfTrees[name1] = newMap;
    return;
  }
  else if (newName == name2)
  {
    mapOfTrees[name2] = newMap;
    return;
  }

  mapOfTrees[newName] = newMap;
}

void demehin::makeIntersect(std::istream& in, MapOfTrees& mapOfTrees)
{
  std::string newName, lhsName, rhsName;
  in >> newName >> lhsName >> rhsName;
  TreeMap newMap, lhsMap, rhsMap;
  lhsMap = mapOfTrees.at(lhsName);
  rhsMap = mapOfTrees.at(rhsName);
  for (auto&& key: lhsMap)
  {
    if (rhsMap.find(key.first) != rhsMap.end())
    {
      newMap.insert(key);
    }
  }

  if (newName == lhsName)
  {
    mapOfTrees[lhsName] = newMap;
    return;
  }
  else if (newName == rhsName)
  {
    mapOfTrees[rhsName] = newMap;
    return;
  }
  mapOfTrees[newName] = newMap;
}

void demehin::makeUnion(std::istream& in, MapOfTrees& mapOfTrees)
{
  std::string newName, lhsName, rhsName;
  in >> newName >> lhsName >> rhsName;
  TreeMap newMap, lhsMap, rhsMap;
  lhsMap = mapOfTrees.at(lhsName);
  rhsMap = mapOfTrees.at(rhsName);
  for (auto&& key: lhsMap)
  {
    newMap.insert(key);
  }
  for (auto&& key: rhsMap)
  {
    if (newMap.find(key.first) == newMap.end())
    {
      newMap.insert(key);
    }
  }

  if (newName == lhsName)
  {
    mapOfTrees[lhsName] = newMap;
    return;
  }
  else if (newName == rhsName)
  {
    mapOfTrees[rhsName] = newMap;
    return;
  }

  mapOfTrees[newName] = newMap;
}
