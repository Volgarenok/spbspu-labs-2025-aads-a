#include "io-sequences.hpp"
#include <iostream>
#include <iterator>
#include <limits>

namespace
{
  iter_list_ull_t getElement(iter_t it, size_t index);
  std::ostream &outputOneList(std::ostream &out, const list_pair_t &list, size_t size);
  list_ull_t findSums(const list_pair_t &list);

  iter_list_ull_t getElement(iter_t it, size_t index)
  {
    iter_list_ull_t it_data = it->second.cbegin();
    for (size_t i = 0; i < index; i++)
    {
      it_data++;
    }
    return it_data;
  }

  std::ostream &outputOneList(std::ostream &out, const list_pair_t &list, size_t size)
  {
    bool first = true;
    iter_t it = list.cbegin();
    for (size_t len = 0; len < list.getSize(); it++, len++)
    {
      if (size >= it->second.getSize())
      {
        continue;
      }
      iter_list_ull_t it_data = getElement(it, size);
      out << (first ? "" : " ") << *it_data;
      first = false;
    }
    return out;
  }

  list_ull_t findSums(const list_pair_t &list)
  {
    size_t num_new_lists = sveshnikov::getMaxSizeLists(list);
    list_ull_t list_sums;
    for (size_t size_list = 0; size_list < num_new_lists; size_list++)
    {
      unsigned long long sum = 0;
      iter_t it = list.cbegin();
      for (size_t len = 0; len < list.getSize(); it++, len++)
      {
        if (size_list >= it->second.getSize())
        {
          continue;
        }
        iter_list_ull_t it_data = getElement(it, size_list);
        if (sum > std::numeric_limits<unsigned long long>::max() - *it_data)
        {
          throw std::overflow_error("ERROR: Overflow when calculating the amount!");
        }
        sum += *it_data;
      }
      list_sums.push_back(sum);
    }
    if (list_sums.empty())
    {
      list_sums.push_back(0);
    }
    return list_sums;
  }
}

void sveshnikov::inputLists(std::istream &in, list_pair_t &list)
{
  std::string name_sequence;
  while (in >> name_sequence)
  {
    list_ull_t data_sequence;
    unsigned long long num = 0;
    while (in >> num)
    {
      data_sequence.push_back(num);
    }
    if (in.fail() && !in.eof())
    {
      in.clear();
    }
    pair_t sequence = {name_sequence, data_sequence};
    list.push_back(sequence);
  }
}

std::ostream &sveshnikov::outputNamesLists(std::ostream &out, const list_pair_t &list)
{
  out << list.cbegin()->first;
  for (iter_t it = ++list.cbegin(); it != list.cend(); it++)
  {
    out << " " << it->first;
  }
  return out;
}

std::ostream &sveshnikov::outputNewLists(std::ostream &out, const list_pair_t &list)
{
  size_t num_new_lists = getMaxSizeLists(list);
  size_t size_list = 0;
  while (size_list + 1 < num_new_lists)
  {
    outputOneList(out, list, size_list) << '\n';
    size_list++;
  }
  outputOneList(out, list, size_list);
  return out;
}

std::ostream &sveshnikov::outputSumsNewLists(std::ostream &out, const list_pair_t &list)
{
  list_ull_t list_sums = findSums(list);
  out << *list_sums.cbegin();
  for (iter_list_ull_t it = ++list_sums.cbegin(); it != list_sums.cend(); it++)
  {
    out << " " << *it;
  }
  return out;
}

size_t sveshnikov::getMaxSizeLists(const list_pair_t &list)
{
  size_t num_lines = list.cbegin()->second.getSize();
  for (iter_t it = ++list.cbegin(); it != list.cend(); it++)
  {
    num_lines = std::max(num_lines, it->second.getSize());
  }
  return num_lines;
}
