#include "commands.hpp"
#include <algorithm>

void mozhegova::print(std::ostream & out, std::string name, const dictionaries & dicts)
{
  std::map< int, std::string > dict = dicts.at(name);
  if (dict.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  out << name;
  for (auto cit = dict.cbegin(); cit != dict.cend(); ++cit)
  {
    out << ' ' << cit->first << ' ' << cit->second;
  }
  out << '\n';
}

void mozhegova::complement(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts)
{
  std::map< int, std::string > dict1 = dicts.at(dictName1);
  std::map< int, std::string > dict2 = dicts.at(dictName2);
  std::map< int, std::string > newDict;
  for (auto cit = dict1.cbegin(); cit != dict1.cend(); ++cit)
  {
    if (!dict2.count(cit->first))
    {
      newDict.insert(*cit);
    }
  }
  dicts[newDictName] = newDict;
}

void mozhegova::intersect(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts)
{
  std::map< int, std::string > dict1 = dicts.at(dictName1);
  std::map< int, std::string > dict2 = dicts.at(dictName2);
  std::map< int, std::string > newDict;
  for (auto cit = dict1.cbegin(); cit != dict1.cend(); ++cit)
  {
    if (dict2.count(cit->first))
    {
      newDict.insert(*cit);
    }
  }
  dicts[newDictName] = newDict;
}

void mozhegova::unionCmd(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts)
{
  std::map< int, std::string > dict1 = dicts.at(dictName1);
  std::map< int, std::string > dict2 = dicts.at(dictName2);
  std::map< int, std::string > newDict = dict1;
  for (auto cit = dict2.cbegin(); cit != dict2.cend(); ++cit)
  {
    if (!dict1.count(cit->first))
    {
      newDict.insert(*cit);
    }
  }
  dicts[newDictName] = newDict;
}
