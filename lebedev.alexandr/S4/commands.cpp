#include "commands.hpp"
#include <string>
#include <iostream>


void lebedev::printCMD(std::ostream& out, const dictOfDicts& dict)
{
  std::string name = "";
  std::cin >> name;
  const lebedev::AVLtree< int, std::string >& current = dict.at(name);
  if (current.empty())
  {
    out << "<EMPTY>";
    return;
  }
  out << name;
  for (auto i = current.cBegin(); i != current.cEnd(); ++i)
  {
    out << " " << (*i).first << " " << (*i).second;
  }
  out << "\n";
}

void lebedev::complementCMD(dictOfDicts& dict)
{
  std::string name, dictName1, dictName2;
  std::cin >> name >> dictName1 >> dictName2;
  auto dict1 = dict.at(dictName1);
  auto dict2 = dict.at(dictName2);
  lebedev::AVLtree< int, std::string > temp;
  for (auto i = dict1.begin(); i != dict1.end(); ++i)
  {
    int key = (*i).first;
    if (dict2.find(key) == dict2.end())
    {
      temp.insert(*i);
    }
  }
  dict[name] = temp;
}

void lebedev::intersectCMD(dictOfDicts& dict)
{
  std::string name, dictName1, dictName2;
  std::cin >> name >> dictName1 >> dictName2;
  auto dict1 = dict.at(dictName1);
  auto dict2 = dict.at(dictName2);
  lebedev::AVLtree< int, std::string > temp;
  for (auto i = dict1.begin(); i != dict1.end(); ++i)
  {
    int key = (*i).first;
    if (dict2.find(key) != dict2.end())
    {
      temp.insert(*i);
    }
  }
  dict[name] = temp;
}

void lebedev::unionCMD(dictOfDicts& dict)
{
  std::string name, dictName1, dictName2;
  std::cin >> name >> dictName1 >> dictName2;
  auto dict1 = dict.at(dictName1);
  auto dict2 = dict.at(dictName2);
  lebedev::AVLtree< int, std::string > temp;
  for (auto i = dict1.begin(); i != dict1.end(); ++i)
  {
    temp.insert(*i);
  }
  for (auto i = dict2.begin(); i != dict2.end(); ++i)
  {
    int key = (*i).first;
    if (dict1.find(key) == dict1.end())
    {
      temp.insert(*i);
    }
  }
  dict[name] = temp;
}
