#include <algorithm>
#include <exception>
#include <iostream>
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
};

int main()
{
  const uint64_t max_number = std::numeric_limits<uint64_t>::max();
  List<NamedList> data;

  std::string token;
  while (std::cin >> token)
  {
    if (isalpha(token[0]))
    {
      data.push_back(NamedList(token));
      auto& back = data.back();
      back.pos = back.list.end();
    }
    else
    {
      auto& back = data.back();
      back.list.push_back(std::stoull(token));
      back.pos = back.list.begin();
    }
  }

  if (data.empty())
  {
    std::cout << "0" << '\n';
    return 0;
  }

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

  List<uint64_t> sums;
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
        if (sum > max_number - value) {
          std::cerr << "can't count sum, overflow\n";
          return 1;
        }
        sum += value;
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
      sums.push_back(sum);
      std::cout << "\n";
    }
  }
  if (sums.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  for (auto x = sums.begin(); x != sums.end(); ++x) {
    if (x == sums.begin()) {
      std::cout << *x;
    } else {
      std::cout << " " << *x;
    }
  }
  std::cout << "\n";

  return 0;
}
