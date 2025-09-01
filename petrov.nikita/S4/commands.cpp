#include "commands.hpp"

std::ostream & petrov::print(std::ostream & out, std::istream & in, const maintree_t & tree)
{
  std::string dataset_parameter;
  in >> dataset_parameter;
  auto it = tree.find(dataset_parameter);
  if (it == tree.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (it->second.empty())
  {
    out << "<EMPTY>";
    out << "\n";
  }
  else
  {
    auto sub_it = it->second.cbegin();
    out << it->first << " ";
    out << sub_it->first << " " << (sub_it++)->second;
    for (; sub_it != it->second.cend(); ++sub_it)
    {
      out << " " << sub_it->first << " " << sub_it->second;
    }
    out << "\n";
  }
  return out;
}

void petrov::complement(std::istream & in, maintree_t & tree)
{
  std::string new_dataset;
  std::string first_dataset;
  std::string second_dataset;
  in >> new_dataset;
  in >> first_dataset;
  in >> second_dataset;
  auto first_it = tree.find(first_dataset);
  auto second_it = tree.find(second_dataset);
  if (first_it == tree.end() || second_it == tree.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  subtree_t new_subtree(first_it->second);
  for (auto it = second_it->second.cbegin(); it != second_it->second.cend(); ++it)
  {
    if (new_subtree.find(it->first) != new_subtree.end())
    {
      new_subtree.erase(it->first);
    }
    else
    {
      new_subtree.insert({ it->first, it->second });
    }
  }
  if (tree.find(new_dataset) != tree.end())
  {
    tree.erase(new_dataset);
  }
  tree.insert({ new_dataset, new_subtree });
}

void petrov::intersect(std::istream & in, maintree_t & tree)
{
  std::string new_dataset;
  std::string first_dataset;
  std::string second_dataset;
  in >> new_dataset;
  in >> first_dataset;
  in >> second_dataset;
  auto first_it = tree.find(first_dataset);
  auto second_it = tree.find(second_dataset);
  if (first_it == tree.end() || second_it == tree.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto first_sub_it = first_it->second.cbegin();
  auto second_sub_it = second_it->second.cbegin();
  subtree_t new_subtree;
  while (first_sub_it != first_it->second.cend() && second_sub_it != second_it->second.cend())
  {
    if (first_sub_it->first == second_sub_it->first)
    {
      new_subtree.insert({ first_sub_it->first, first_sub_it->second });
      ++first_sub_it;
      ++second_sub_it;
    }
    else if (first_sub_it->first < second_sub_it->first)
    {
      ++first_sub_it;
    }
    else
    {
      ++second_sub_it;
    }
  }
  if (tree.find(new_dataset) != tree.end())
  {
    tree.erase(new_dataset);
  }
  tree.insert({ new_dataset, new_subtree });
}

void petrov::unionCMD(std::istream & in, maintree_t & tree)
{
  std::string new_dataset;
  std::string first_dataset;
  std::string second_dataset;
  in >> new_dataset;
  in >> first_dataset;
  in >> second_dataset;
  auto first_it = tree.find(first_dataset);
  auto second_it = tree.find(second_dataset);
  if (first_it == tree.end() || second_it == tree.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  subtree_t new_subtree(first_it->second);
  for (auto it = second_it->second.cbegin(); it != second_it->second.cend(); ++it)
  {
    if (new_subtree.find(it->first) == new_subtree.end())
    {
      new_subtree.insert({ it->first, it->second });
    }
  }
  if (tree.find(new_dataset) != tree.end())
  {
    tree.erase(new_dataset);
  }
  tree.insert({ new_dataset, new_subtree });
}
