#include "processor.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

rychkov::S4ParseProcessor::S4ParseProcessor(int argc, char** argv)
{
  if (argc != 2)
  {
    throw std::invalid_argument("wrong arguments count");
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    throw std::invalid_argument("failed to open file");
  }
  std::string name;
  while (file >> name)
  {
    if (map.contains(name))
    {
      throw std::runtime_error("map name repeated");
    }
    inner_map& link = map[name];
    int key = 0;
    while (file >> key)
    {
      std::string str;
      if (!(file >> str))
      {
        throw std::runtime_error("failed to read string for key");
      }
      link.try_emplace(key, std::move(str));
    }
    file.clear(file.rdstate() & ~std::ios::failbit);
  }
}
bool rychkov::S4ParseProcessor::print(ParserContext& context)
{
  std::string name;
  if (!(context.in >> name) || !context.eol())
  {
    return false;
  }
  outer_map::const_iterator data_p = map.find(name);
  if (data_p == map.end())
  {
    return false;
  }
  if (data_p->second.empty())
  {
    context.out << "<EMPTY>\n";
    return true;
  }
  context.out << data_p->first;
  for (const inner_map::value_type& i: data_p->second)
  {
    context.out << ' ' << i.first << ' ' << i.second;
  }
  context.out << '\n';
  return true;
}
bool rychkov::S4ParseProcessor::make_complement(ParserContext& context)
{
  std::string name, lhs, rhs;
  if (!(context.in >> name >> lhs >> rhs) || !context.eol() || !map.contains(lhs) || !map.contains(rhs))
  {
    return false;
  }
  inner_map& link = map[name];
  const inner_map& lhslink = map.at(lhs);
  const inner_map& rhslink = map.at(rhs);
  inner_map temp;
  for (const inner_map::value_type& i: lhslink)
  {
    if (!rhslink.contains(i.first))
    {
      temp.insert(i);
    }
  }
  link = std::move(temp);
  return true;
}
bool rychkov::S4ParseProcessor::make_intersect(ParserContext& context)
{
  std::string name, lhs, rhs;
  if (!(context.in >> name >> lhs >> rhs) || !context.eol() || !map.contains(lhs) || !map.contains(rhs))
  {
    return false;
  }
  inner_map& link = map[name];
  const inner_map& lhslink = map.at(lhs);
  const inner_map& rhslink = map.at(rhs);
  inner_map temp;
  for (const inner_map::value_type& i: lhslink)
  {
    if (rhslink.contains(i.first))
    {
      temp.insert(i);
    }
  }
  link = std::move(temp);
  return true;
}
bool rychkov::S4ParseProcessor::make_union(ParserContext& context)
{
  std::string name, lhs, rhs;
  if (!(context.in >> name >> lhs >> rhs) || !context.eol() || !map.contains(lhs) || !map.contains(rhs))
  {
    return false;
  }
  inner_map& link = map[name];
  const inner_map& lhslink = map.at(lhs);
  const inner_map& rhslink = map.at(rhs);
  inner_map temp = lhslink;
  temp.insert(rhslink.begin(), rhslink.end());
  link = std::move(temp);
  return true;
}
