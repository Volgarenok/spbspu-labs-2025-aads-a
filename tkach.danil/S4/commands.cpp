#include "commands.hpp"
#include <cstddef>

void tkach::print(std::istream& in, const AvlTree< std::string, AvlTree< size_t, std::string > >& avltree, std::ostream& out)
{
  std::string name;
  in >> name;
  AvlTree< size_t, std::string > root = avltree.at(name);
  if (root.empty())
  {
    out << "<EMPTY>\n";
  }
  else
  {
    out << name;
    for (auto it = root.cbegin(); it != root.cend(); ++it)
    {
      out << " " << (*it).first << " " << (*it).second;
    }
    out << "\n";
  }
}

void tkach::intersect(std::istream& in, AvlTree< std::string, AvlTree< size_t, std::string > >& avltree)
{
  std::string new_name;
  std::string first_name;
  std::string last_name;
  in >> new_name >> first_name >> last_name;
  AvlTree< size_t, std::string > root;
  const AvlTree< size_t, std::string > first = avltree.at(first_name);
  const AvlTree< size_t, std::string > second = avltree.at(last_name);
  for (auto it = first.cbegin(); it != first.cend(); ++it)
  {
    if (second.find(it->first) != second.cend())
    {
      root.insert(std::make_pair(it->first, it->second));
    }
  }
  try
  {
    avltree.at(new_name) = root;
  }
  catch (const std::out_of_range&)
  {
    avltree.insert(std::make_pair(new_name, root));
  }
}

void tkach::complement(std::istream& in, AvlTree< std::string, AvlTree< size_t, std::string > >& avltree)
{
  std::string new_name;
  std::string first_name;
  std::string last_name;
  in >> new_name >> first_name >> last_name;
  AvlTree< size_t, std::string > root;
  const AvlTree< size_t, std::string > first = avltree.at(first_name);
  const AvlTree< size_t, std::string > second = avltree.at(last_name);
  for (auto it = first.cbegin(); it != first.cend(); ++it)
  {
    if (second.find(it->first) == second.cend())
    {
      root.insert({it->first, it->second});
    }
  }
  for (auto it = second.cbegin(); it != second.cend(); ++it)
  {
    if (first.find(it->first) == first.cend())
    {
      root.insert({it->first, it->second});
    }
  }
  try
  {
    avltree.at(new_name) = root;
  }
  catch (const std::out_of_range&)
  {
    avltree.insert(std::make_pair(new_name, root));
  }
}

void tkach::unionTree(std::istream& in, AvlTree< std::string, AvlTree< size_t, std::string > >& avltree)
{
  std::string new_name;
  std::string first_name;
  std::string last_name;
  in >> new_name >> first_name >> last_name;
  AvlTree< size_t, std::string > root;
  const AvlTree< size_t, std::string > first = avltree.at(first_name);
  const AvlTree< size_t, std::string > second = avltree.at(last_name);
  for (auto it = first.cbegin(); it != first.cend(); ++it)
  {
    root.insert({it->first, it->second});
  }
  for (auto it = second.cbegin(); it != second.cend(); ++it)
  {
    if (root.find(it->first) == root.end())
    {
      root.insert({it->first, it->second});
    }
  }
  try
  {
    avltree.at(new_name) = root;
  }
  catch (const std::out_of_range&)
  {
    avltree.insert(std::make_pair(new_name, root));
  }
}

