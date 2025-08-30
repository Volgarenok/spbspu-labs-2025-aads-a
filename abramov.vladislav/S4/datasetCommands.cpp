#include "datasetCommands.hpp"
#include <iostream>
#include <limits>

abramov::collections abramov::getDataSets(std::ifstream &in)
{
  collections collection;
  while (in)
  {
    std::string name;
    in >> name;
    tree dict;
    int k;
    std::string s;
    while(in && in.peek() != '\n')
    {
      in >> k >> s;
      dict[k] = s;
    }
    collection[name] = dict;
  }
  return collection;
}

void abramov::printDataSet(const std::string &name, const collections &dicts)
{
  if (dicts.cfind(name) == dicts.cend())
  {
    throw std::logic_error("No dictionary\n");
  }
  const tree &dict = dicts.cfind(name)->second;
  if (dict.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }
  std::cout << name << " ";
  for (auto it = dict.cbegin(); it != --dict.cend(); ++it)
  {
    std::cout << it->first << " " << it->second << " ";
  }
  std::cout << (--dict.cend())->first << " " << (--dict.cend())->second << "\n";
}

void abramov::complementDataSets(const std::string &s, const std::string &s1, const std::string &s2, collections &dicts)
{
  if (dicts.cfind(s1) == dicts.cend() || dicts.cfind(s2) == dicts.cend())
  {
    throw std::logic_error("No such dictionaries\n");
  }
  const tree &dict1 = dicts.cfind(s1)->second;
  const tree &dict2 = dicts.cfind(s2)->second;
  tree res = tree();
  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    if (dict2.cfind(it->first) == dict2.cend())
    {
      res[it->first] = it->second;
    }
  }
  dicts[s] = res;
}

void abramov::intersectDataSets(const std::string &s, const std::string &s1, const std::string &s2, collections &dicts)
{
  if (dicts.cfind(s1) == dicts.cend() || dicts.cfind(s2) == dicts.cend())
  {
    throw std::logic_error("No such dictionaries\n");
  }
  const tree &dict1 = dicts.cfind(s1)->second;
  const tree &dict2 = dicts.cfind(s2)->second;
  tree res = tree();
  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    if (dict2.cfind(it->first) != dict2.cend())
    {
      res[it->first] = it->second;
    }
  }
  dicts[s] = res;
}

void abramov::unionDataSets(const std::string &s, const std::string &s1, const std::string &s2, collections &dicts)
{
  if (dicts.cfind(s1) == dicts.cend() || dicts.cfind(s2) == dicts.cend())
  {
    throw std::logic_error("No such dictionaries\n");
  }
  const tree &dict1 = dicts.cfind(s1)->second;
  const tree &dict2 = dicts.cfind(s2)->second;
  tree res = tree();
  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    res[it->first] = it->second;
  }
  for (auto it = dict2.cbegin(); it != dict2.cend(); ++it)
  {
    if (res.cfind(it->first) == res.cend())
    {
      res[it->first] = it->second;
    }
  }
  dicts[s] = res;
}

void abramov::doCommand(const std::string &s, std::istream &in, collections &dicts)
{
  std::string s1;
  std::string s2;
  std::string name;
  if (s == "print")
  {
    in >> s1;
    printDataSet(s1, dicts);
  }
  else if (s == "complement")
  {
    in >> name >> s1 >> s2;
    complementDataSets(name, s1, s2, dicts);
  }
  else if (s == "intersect")
  {
    in >> name >> s1 >> s2;
    intersectDataSets(name, s1, s2, dicts);
  }
  else if (s == "union")
  {
    in >> name >> s1 >> s2;
    unionDataSets(name, s1, s2, dicts);
  }
  else
  {
    if (std::cin.eof())
    {
      return;
    }
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
