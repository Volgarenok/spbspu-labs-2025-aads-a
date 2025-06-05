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
  List<uint64_t> list;
  List<uint64_t>::iterator pos;

  NamedList(const std::string& name) : name(name) {}

  void reset() { pos = list.begin(); }
};

void inputData(List<NamedList>& data, std::istream& ist)
{
  std::string token;
  while (ist >> token)
  {
    if (isalpha(token[0]))
    {
      data.push_back(NamedList(token));
    }
    else
    {
      auto& back = data.back();
      back.list.push_back(std::stoull(token));
    } 
  }
}

void printDataNames(const List<NamedList>& data)
{
  for (auto x = data.begin(); x != data.end(); ++x)
  {
    if (x == data.begin())
    {
      std::cout << x->name;
    }
    else
    {
      std::cout << " " << x->name;
    }
  }
  std::cout << "\n";
}

void printDataValues(List<NamedList>& data)
{
  for (auto& x : data)
  {
    x.reset();
  }
  bool flag = true;
  while (flag)
  {
    flag = false;
    for (auto& x : data)
    {
      if (x.pos != x.list.end())
      {
        uint64_t value = *(x.pos);
        ++(x.pos);
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

void countSums(List<NamedList>& data, List<uint64_t>& sums)
{
  const uint64_t max_number = std::numeric_limits<uint64_t>::max();
  for (auto& x : data)
  {
    x.reset();
  }
  bool flag = true;
  while (flag)
  {
    flag = false;
    uint64_t sum = 0;
    for (auto& x : data)
    {
      if (x.pos != x.list.end())
      {
        uint64_t value = *(x.pos);
        ++(x.pos);
        if (sum > max_number - value)
        {
          throw std::logic_error("can't count sum, overflow");
        }
        sum += value;
        flag = true;
      }
    }
    if (flag)
    {
      sums.push_back(sum);
    }
  }
}

template <typename T>
void printList(T first, T last)
{
  if (first == last)
  {
    std::cout << "0\n";
    return;
  }
  for (T x = first; x != last; ++x)
  {
    if (x == first)
    {
      std::cout << *x;
    }
    else
    {
      std::cout << " " << *x;
    }
  }
  std::cout << "\n";
}

int main()
{
  List<NamedList> data;

  inputData(data, std::cin);
  if (data.empty())
  {
    std::cout << "0" << '\n';
    return 0;
  }

  printDataNames(data);
  printDataValues(data);

  List<uint64_t> sums;
  try
  {
    countSums(data, sums);
  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what() << "\n";
    return 1;
  }
  printList(sums.begin(), sums.end());

  return 0;
}
