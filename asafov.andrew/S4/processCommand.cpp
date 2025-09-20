#include "command.h"
#include <iostream>
#include <sstream>

void asafov::printMap(const maps_t& maps, const std::string& name)
{
  auto it = maps.find(name);
  if (it == maps.end())
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if ((*it).second.empty())
  {
    std::cout << "<EMPTY>" << std::endl;
    return;
  }

  std::cout << name;
  for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
  {
    std::cout << " " << (*it2).first << " " << (*it2).second;
  }
  std::cout << std::endl;
}

bool asafov::complement(maps_t& maps, const std::string& newName, const std::string& left, const std::string& right)
{
  if (maps.find(left) == maps.end() || maps.find(right) == maps.end())
  {
    return false;
  }

  map_t result;
  const map_t& leftMap = maps[left];
  const map_t& rightMap = maps[right];

  for (auto it = leftMap.begin(); it != leftMap.end(); ++it)
  {
    if (rightMap.find((*it).first) == rightMap.end())
    {
      result[(*it).first] = (*it).second;
    }
  }

  maps[newName] = result;
  return true;
}

bool asafov::intersect(maps_t& maps, const std::string& newName, const std::string& left, const std::string& right)
{
  if (maps.find(left) == maps.end() || maps.find(right) == maps.end())
  {
    return false;
  }

  map_t result;
  const map_t& leftMap = maps[left];
  const map_t& rightMap = maps[right];

  for (auto it = leftMap.begin(); it != leftMap.end(); ++it)
  {
    if (rightMap.find((*it).first) != rightMap.end())
    {
      result[(*it).first] = (*it).second;
    }
  }

  maps[newName] = result;
  return true;
}

bool asafov::unionMaps(maps_t& maps, const std::string& newName, const std::string& left, const std::string& right)
{
  if (maps.find(left) == maps.end() || maps.find(right) == maps.end())
  {
    return false;
  }

  map_t result = maps[left];
  const map_t& rightMap = maps[right];

  for (auto it = rightMap.begin(); it != rightMap.end(); ++it)
  {
    if (result.find((*it).first) == result.end())
    {
      result[(*it).first] = (*it).second;
    }
  }

  maps[newName] = result;
  return true;
}

bool asafov::processCommand(maps_t& maps, const std::string& line)
{
  std::istringstream ss(line);
  std::string command;
  ss >> command;

  if (command == "print")
  {
    std::string name;
    if (ss >> name)
    {
      printMap(maps, name);
      return true;
    }
  }
  else if (command == "complement")
  {
    std::string newName, left, right;
    if (ss >> newName >> left >> right)
    {
      if (!complement(maps, newName, left, right))
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      return true;
    }
  }
  else if (command == "intersect")
  {
    std::string newName, left, right;
    if (ss >> newName >> left >> right)
    {
      if (!intersect(maps, newName, left, right))
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      return true;
    }
  }
  else if (command == "union")
  {
    std::string newName, left, right;
    if (ss >> newName >> left >> right)
    {
      if (!unionMaps(maps, newName, left, right))
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      return true;
    }
  }

  std::cout << "<INVALID COMMAND>\n";
  return false;
}
