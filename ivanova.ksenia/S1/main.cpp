#include <algorithm>
#include <exception>
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
      try
      {
        back.list.push_back(std::stoi(token));
      }
      catch (const std::out_of_range& e)
      {
        std::cerr << e.what() << ": number is to big for int\n";
        return 1;
      }
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
        int value = *(x.pos);
        ++(x.pos);
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
  std::copy(sums.begin(), sums.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << "\n";

  return 0;
}
