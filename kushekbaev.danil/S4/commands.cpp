#include "commands.hpp"
#include <algorithm>
#include <vector.hpp>

using dataset_t = kushekbaev::Tree< size_t, std::string >;
using dict_t = kushekbaev::Tree< std::string, dataset_t >;

void kushekbaev::print(std::ostream& out, std::istream& in, const dict_t& dictionary)
{
  if (dictionary.empty())
  {
    out << "<EMPTY>";
    return;
  }
  std::string name;
  in >> name;
  dataset_t result = dictionary.at(name);
  if (result.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  kushekbaev::Vector< std::pair < size_t, std::string > > sorted;
  for (const auto& elem: result)
  {
    sorted.pushBack(elem);
  }
  std::sort(std::addressof(sorted[0]), std::addressof(sorted[0]) + sorted.size());
  out << name;
  for (size_t i = 0; i < sorted.size(); ++i)
  {
    out << " " << sorted[i].first << " " << sorted[i].second;
  }
  out << "\n";
}

void kushekbaev::complement(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  dataset_t result;
  if (name1 == name2)
  {
    dictionary[newName] = result;
    return;
  }
  const dataset_t ds1 = dictionary.at(name1);
  const dataset_t ds2 = dictionary.at(name2);
  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it)
  {
    if (ds2.find(it->first) == ds2.cend())
    {
      result.insert(*it);
    }
  }
  dictionary[newName] = result;
}

void kushekbaev::intersect(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  dataset_t result;
  const dataset_t ds1 = dictionary.at(name1);
  const dataset_t ds2 = dictionary.at(name2);
  if (name1 == name2)
  {
    result = ds1;
    dictionary[newName] = result;
    return;
  }
  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it)
  {
    if (ds2.find(it->first) != ds2.cend())
    {
      result.insert(*it);
    }
  }
  dictionary[newName] = result;
}

void kushekbaev::unification(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  const dataset_t ds1 = dictionary.at(name1);
  const dataset_t ds2 = dictionary.at(name2);
  dataset_t result;
  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it)
  {
    result.insert(*it);
  }
  for (auto it = ds2.cbegin(); it != ds2.cend(); ++it)
  {
    if (result.find(it->first) == result.end())
    {
      result.insert(*it);
    }
  }
  dictionary[newName] = result;
}
