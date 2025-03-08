#include "io-sequences.hpp"
#include <iostream>
#include <iterator>
#include <limits>
#include "list-manip.hpp"

namespace
{
  void inputOneList(std::istream &in, list_ull_t &data);
  std::ostream &outputOneList(std::ostream &out, const list_pair_t &list, size_t size);
  list_ull_t findSums(const list_pair_t &list);

  void inputOneList(std::istream &in, list_ull_t &data)
  {
    unsigned long long num = 0;
    iter_list_ull_t it = data.before_begin();
    while (in >> num)
    {
      data.insert_after(it, num);
      it++;
    }
    if (in.fail() && !in.eof())
    {
      in.clear();
    }
  }

  std::ostream &outputOneList(std::ostream &out, const list_pair_t &list, size_t size)
  {
    bool first = true;
    for (iter_t it = list.cbegin(); it != list.cend(); it++)
    {
      if (size >= sveshnikov::getSizeDataList(it))
      {
        continue;
      }
      iter_list_ull_t it_data = it->second.cbegin();
      for (size_t i = 0; i < size; i++)
      {
        it_data++;
      }
      if (first)
      {
        first = false;
        out << *it_data;
        continue;
      }
      out << " " << *it_data;
    }
    return out;
  }

  list_ull_t findSums(const list_pair_t &list)
  {
    size_t num_new_lists = sveshnikov::getMaxSizeLists(list);
    list_ull_t list_sums;
    iter_list_ull_t it_sums = list_sums.before_begin();
    for (size_t size_list = 0; size_list < num_new_lists; size_list++)
    {
      unsigned long long sum = 0;
      for (iter_t it = list.cbegin(); it != list.cend(); it++)
      {
        if (size_list >= sveshnikov::getSizeDataList(it))
        {
          continue;
        }
        iter_list_ull_t it_data = it->second.cbegin();
        for (size_t i = 0; i < size_list; i++)
        {
          it_data++;
        }
        if (sum > std::numeric_limits<unsigned long long>::max() - *it_data)
        {
          list_sums.clear();
          throw std::overflow_error("ERROR: Overflow when calculating the amount!");
        }
        sum += *it_data;
      }
      try
      {
        list_sums.insert_after(it_sums, sum);
      }
      catch (const std::bad_alloc &e)
      {
        list_sums.clear();
        throw;
      }
      it_sums++;
    }
    if (list_sums.empty())
    {
      list_sums.push_front(0);
    }
    return list_sums;
  }
}

void sveshnikov::inputLists(std::istream &in, list_pair_t &list)
{
  std::string name_sequence;
  iter_t it = list.before_begin();
  while (in >> name_sequence)
  {
    list_ull_t data_sequence;
    inputOneList(in, data_sequence);
    pair_t sequence = {name_sequence, data_sequence};
    list.insert_after(it, sequence);
    it++;
  }
  if (list.empty())
  {
    return;
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
