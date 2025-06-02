#include <algorithm>
#include <iostream>
#include <string>
#include "list.hpp"

using ivanova::List;

struct NamedList
{
  std::string name;
  List<int> list;
  List<int>::iterator pos;

  NamedList(const std::string& name) : name(name) {}
};

int main()
{
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
      back.list.push_back(std::stoi(token));
      back.pos = back.list.begin();
    }
  }

  if (data.empty())
  {
    std::cout << "0" << '\n';
    return 0;
  }

  for (const auto& x : data)
  {
    std::cout << x.name << " ";
  }
  std::cout << "\n";

  List<int> sums;
  bool flag = true;
  while (flag)
  {
    flag = false;
    int sum = 0;
    for (auto& x : data)
    {
      if (x.pos != x.list.end())
      {
        flag = true;
        int value = *(x.pos);
        ++(x.pos);
        sum += value;
        std::cout << value << " ";
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
    std::cerr << "Can't count sums: all sequences are empty" << '\n';
    return 1;
  }
  for (const auto& x : sums)
  {
    std::cout << x << " ";
  }
  std::cout << "\n";

  return 0;
}
