#include <iostream>
#include <string>
#include <cstddef>
#include <limits>
#include "list.hpp"

namespace
{
  using pairs = std::pair< std::string, tkach::List< size_t > >;

  tkach::List< pairs > inputLists(std::istream& in)
  {
    tkach::List< pairs > fd_pair_list;
    std::string list_name = "";
    while (in >> list_name)
    {
      if (list_name.empty())
      {
        continue;
      }
      tkach::List < size_t > temp_list;
      size_t num = 0;
      while (in >> num)
      {
        temp_list.pushBack(num);
      }
      fd_pair_list.pushBack(std::make_pair(list_name, temp_list));
      in.clear();
    }
    return fd_pair_list;
  }

  void outputListNames(std::ostream& out, const tkach::List< pairs >& list)
  {
    out << (list.cbegin())->first;
    for (auto it = ++list.cbegin(); it != list.cend(); ++it)
    {
      out << " " << it->first;
    }
  }

  void outputListDataAndSums(std::ostream& out, const tkach::List< pairs >& list)
  {
    tkach::List< pairs > fd_pair_list(list);
    auto max = std::numeric_limits< size_t >::max();
    bool overflow = false;
    tkach::List< size_t > sums;
    tkach::List< size_t > new_list;
    new_list.pushFront(0);
    while (!new_list.empty())
    {
      new_list.clear();
      size_t sum = 0;
      auto it = fd_pair_list.begin();
      do
      {
        if (it->second.empty())
        {
          it++;
          continue;
        }
        size_t pair_second_front = it->second.front();
        if (sum > max - pair_second_front)
        {
          overflow = true;
        }
        sum += pair_second_front;
        new_list.pushBack(pair_second_front);
        it->second.popFront();
        it++;
      }
      while (it != fd_pair_list.begin());
      if (!new_list.empty())
      {
        out << new_list.front();
        for (auto it = ++new_list.begin(); it != new_list.end(); ++it)
        {
          out << " " << *it;
        }
        out << "\n";
        if (sum != 0)
        {
          sums.pushBack(sum);
        }
      }
    }
    if (sums.empty())
    {
      out << 0;
      return;
    }
    if (overflow == true)
    {
      throw std::overflow_error("Error: overflow");
    }
    out << sums.front();
    for (auto it = ++sums.begin(); it != sums.end(); ++it)
    {
      out << " " << *it;
    }
  }

  void outputAll(std::ostream& out, const tkach::List< pairs >& fd_pair_list)
  {
    outputListNames(out, fd_pair_list);
    out << "\n";
    outputListDataAndSums(out, fd_pair_list);
  }

}

int main()
{
  using namespace tkach;
  try
  {
    List< std::pair< std::string, List < size_t > > > fd_pair_list = inputLists(std::cin);
    if (fd_pair_list.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    outputAll(std::cout, fd_pair_list);
    std::cout << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
