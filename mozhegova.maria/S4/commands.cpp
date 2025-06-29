#include "commands.hpp"
#include <algorithm>
#include <string>

void mozhegova::print(std::istream & in, std::ostream & out, const dictionaries & dicts)
{
  std::string name;
  in >> name;
  BiTree< int, std::string > dict = dicts.at(name);
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

void mozhegova::complement(std::istream & in, dictionaries & dicts)
{
  std::string dictName1, dictName2, newDictName;
  in >> newDictName >> dictName1 >> dictName2;
  BiTree< int, std::string > dict1 = dicts.at(dictName1);
  BiTree< int, std::string > dict2 = dicts.at(dictName2);
  BiTree< int, std::string > newDict;
  for (auto cit = dict1.cbegin(); cit != dict1.cend(); ++cit)
  {
    if (!dict2.count(cit->first))
    {
      newDict.insert(*cit);
    }
  }
  dicts[newDictName] = newDict;
}

void mozhegova::intersect(std::istream & in, dictionaries & dicts)
{
  std::string dictName1, dictName2, newDictName;
  in >> newDictName >> dictName1 >> dictName2;
  BiTree< int, std::string > dict1 = dicts.at(dictName1);
  BiTree< int, std::string > dict2 = dicts.at(dictName2);
  BiTree< int, std::string > newDict;
  for (auto cit = dict1.cbegin(); cit != dict1.cend(); ++cit)
  {
    if (dict2.count(cit->first))
    {
      newDict.insert(*cit);
    }
  }
  dicts[newDictName] = newDict;
}

void mozhegova::unionCmd(std::istream & in, dictionaries & dicts)
{
  std::string dictName1, dictName2, newDictName;
  in >> newDictName >> dictName1 >> dictName2;
  BiTree< int, std::string > dict1 = dicts.at(dictName1);
  BiTree< int, std::string > dict2 = dicts.at(dictName2);
  BiTree< int, std::string > newDict = dict1;
  for (auto cit = dict2.cbegin(); cit != dict2.cend(); ++cit)
  {
    if (!dict1.count(cit->first))
    {
      newDict.insert(*cit);
    }
  }
  dicts[newDictName] = newDict;
}
