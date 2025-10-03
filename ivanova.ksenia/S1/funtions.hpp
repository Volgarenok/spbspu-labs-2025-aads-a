#include <algorithm>
#include <exception>
#include <iostream>
#include <cstdint>
#include <limits>
#include <string>
#include "list.hpp"

using ivanova::List;

struct NamedList
{
  std::string name;
  List< uint64_t > list;
  List< uint64_t >::iterator pos;

  NamedList(const std::string& name):
    name(name),
    pos(list.begin())
  {}

  void reset()
  {
    pos = list.begin();
  }
};

void inputData(List< NamedList >& data, std::istream& ist)
{
  std::string token;
  while (ist >> token)
  {
    if (std::isalpha(token[0]))
    {
      data.push_back(NamedList(token));
    }
    else
    {
      if (data.empty())
      {
        continue;
      }
      NamedList& back = data.back();
      back.list.push_back(std::stoull(token));
    }
  }
}

void printDataNames(const List< NamedList >& data)
{
  if (data.empty())
  {
    std::cout << "\n";
    return;
  }

  auto it = data.begin();
  std::cout << it->name;
  ++it;

  for (; it != data.end(); ++it)
  {
    std::cout << " " << it->name;
  }
  std::cout << "\n";
}

void printDataValues(List< NamedList >& data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    it->reset();
  }

  bool hasValues = true;
  while (hasValues)
  {
    hasValues = false;
    bool firstValue = true;

    for (auto it = data.begin(); it != data.end(); ++it)
    {
      if (it->pos != it->list.end())
      {
        if (!firstValue)
        {
          std::cout << " ";
        }
        std::cout << *(it->pos);
        ++(it->pos);
        hasValues = true;
        firstValue = false;
      }
    }

    if (hasValues)
    {
      std::cout << "\n";
    }
  }
}

void countSums(List< NamedList >& data, List< uint64_t >& sums)
{
  const uint64_t max_number = std::numeric_limits< uint64_t >::max();

  for (auto it = data.begin(); it != data.end(); ++it)
  {
    it->reset();
  }

  bool hasElements = true;
  while (hasElements)
  {
    uint64_t sum = 0;
    hasElements = false;

    for (auto it = data.begin(); it != data.end(); ++it)
    {
      if (it->pos != it->list.end())
      {
        uint64_t value = *(it->pos);
        ++(it->pos);
        
        if (sum > max_number - value)
        {
          throw std::overflow_error("can't count sum, overflow");
        }
        sum += value;
        hasElements = true;
      }
    }

    if (hasElements)
    {
      sums.push_back(sum);
    }
  }
}

template < typename T >
void printList(T first, T last)
{
  if (first == last)
  {
    std::cout << "0\n";
    return;
  }

  std::cout << *first;
  while (++first != last)
  {
    std::cout << ' ' << *first;
  }
  std::cout << '\n';
}
