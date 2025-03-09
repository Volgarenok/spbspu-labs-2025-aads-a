#include "inputProcess.hpp"
#include <iostream>
#include <list>
#include <sstream>
#include <cctype>
#include <limits>

using namespace alymova;

void alymova::inputProcess(std::istream& in, list_pair_t& list)
{
  std::string name;
  while ((in >> name) && (!in.eof()))
  {
    list_int_t list_int;
    size_t num;
    while ((in >> num))
    {
      list_int.push_back(num);
    }
    pair_t p(name, list_int);
    list.push_back(p);
    if (in.eof())
    {
      break;
    }
    in.clear();
  }
}

void alymova::outputProcess(std::ostream& out, const list_pair_t& list)
{
  size_t max_size = findMaxListSize(list);
  for (size_t i = 0; i < max_size; i++)
  {
    bool printed = false;
    for (auto it = list.cbegin(); it != list.cend(); it++)
    {
      const list_int_t list_now = (*it).second;
      if (list_now.size() > i)
      {
        auto list_now_it = list_now.cbegin();
        for (size_t j = 0; j < i; j++)
        {
          list_now_it++;
        }
        if (printed)
        {
          out << " ";
        }
        out << *list_now_it;
        printed = true;
      }
    }
    out << "\n";
  }
}

list_int_t alymova::countSums(const list_pair_t& list)
{
  list_int_t sums;
  size_t max_size = findMaxListSize(list);
  for (size_t i = 0; i < max_size; i++)
  {
    size_t sum_now = 0;
    for (auto it = list.cbegin(); it != list.cend(); it++)
    {
      const list_int_t list_now = (*it).second;
      if (list_now.size() > i)
      {
        auto list_now_it = list_now.cbegin();
        for (size_t j = 0; j < i; j++)
        {
          list_now_it++;
        }
        if (isOverflowSumInt(sum_now, *list_now_it))
        {
          throw std::logic_error("Summation is incorrect");
        }
        sum_now += (*list_now_it);
      }
    }
    sums.push_back(sum_now);
  }
  return sums;
}

void alymova::outputListInt(std::ostream& out, const list_int_t& list)
{
  for (auto it = list.cbegin(); it != list.cend(); it++)
  {
    if (it != list.cbegin())
    {
      out << " ";
    }
    out << (*it);
  }
}

void alymova::outputListString(std::ostream& out, const list_pair_t& list)
{
  for (auto it = list.cbegin(); it != list.cend(); it++)
  {
    if (it != list.cbegin())
    {
      out << " ";
    }
    out << (*it).first;
  }
}

size_t alymova::findMaxListSize(const list_pair_t& list)
{
  size_t max_size = 0;
  for (auto it = list.cbegin(); it != list.cend(); it++)
  {
    size_t size_now = (*it).second.size();
    max_size = std::max(max_size, size_now);
  }
  return max_size;
}

bool alymova::isOverflowSumInt(size_t a, size_t b)
{
  return (b > std::numeric_limits< size_t >::max() - a);
}
