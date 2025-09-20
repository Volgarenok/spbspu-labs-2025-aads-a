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
    name(name)
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
      NamedList& back = data.back();
      back.list.push_back(std::stoull(token));
    }
  }
}

void printDataNames(const List< NamedList >& data)
{
  List< NamedList >::const_iterator it = data.begin();
  if (it != data.end())
  {
    std::cout << it->name;
    ++it;
  }
  for (; it != data.end(); ++it)
  {
    std::cout << " " << it->name;
  }
  std::cout << "\n";
}

void printDataValues(List< NamedList >& data)
{
  for (List< NamedList >::iterator it = data.begin(); it != data.end(); ++it)
  {
    it->reset();
  }
  bool flag = true;
  while (flag)
  {
    flag = false;
    for (List< NamedList >::iterator it = data.begin(); it != data.end(); ++it)
    {
      if (it->pos != it->list.end())
      {
        size_t value = *(it->pos);
        ++(it->pos);
        if (!flag)
        {
          std::cout << value;
        }
        else
        {
          std::cout << " " << value;
        }
        flag = true;
      }
    }
    if (flag)
    {
      std::cout << "\n";
    }
  }
}

void countSums(List< NamedList >& data, List< uint64_t >& sums)
{
  const uint64_t max_number = std::numeric_limits< uint64_t >::max();
  for (List< NamedList >::iterator it = data.begin(); it != data.end(); ++it)
  {
    it->reset();
  }
  size_t sum = 0;
  do
  {
    sum = 0;
    bool hasElements = false;

    for (List< NamedList >::iterator it = data.begin(); it != data.end(); ++it)
    {
      if (it->pos != it->list.end())
      {
        size_t value = *(it->pos);
        ++(it->pos);
        if (sum > max_number - value)
        {
          throw std::logic_error("can't count sum, overflow");
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
  while (sum > 0);
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

int main()
{
  List< NamedList > data;

  inputData(data, std::cin);
  if (data.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  printDataNames(data);
  printDataValues(data);

  List< uint64_t > sums;
  try
  {
    countSums(data, sums);
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << "\n";
    return 1;
  }
  printList(sums.begin(), sums.end());

  return 0;
}
