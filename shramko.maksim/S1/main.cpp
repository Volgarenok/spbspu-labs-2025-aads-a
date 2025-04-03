#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <limits>


std::size_t maxPairSize(pairs_list_t& list) noexcept
{
  std::size_t max = 0, size = 0;
  for (auto i = list.begin(); i != list.end(); ++i)
  {
    for (auto j = i->second.begin(); j != i->second.end(); ++j)
    {
      ++size;
    }
    max = max > size ? max : size;
  }
  return max;
}

std::size_t getListElemenSum(list_t& list)
{
  std::size_t res = 0;
  for (auto i = list.begin(); i != list.end(); ++i)
  {
    res = getSum(res, *i);
  }
  return res;
}

void printInfo(std::ostream& out, pairs_list_t pairs_list)
{
  pairs_list_t copy = pairs_list;
  std::size_t max_pairs_list_size = maxPairSize(pairs_list);
  list_t sums;

  for (std::size_t i = 0; i != max_pairs_list_size; ++i)
  {
    list_t col;
    for (auto j = pairs_list.begin(); j != pairs_list.end(); ++j)
    {
      if (!j->second.empty())
      {
        col.push_back(j->second.front());
        j->second.pop_front();
      }
    }
    
    if (!col.empty())
    {
      out << *col.begin();

      for (auto j = ++col.begin(); j != col.end(); ++j)
      {
        out << ' ' << *j;
      }

      std::cout << '\n';
      sums.push_back(getListElemenSum(col));
    }
  }

  out << *sums.begin();
  
  for (auto j = ++sums.begin(); j != sums.end(); ++j)
  {
    out << ' ' << *j;
  }
}

int main()
{
  pairs_list_t pairsList;
  std::string listName = "";
  bool isEmpty = true;

  while (std::cin >> listName)
  {
    list_t list;
    unsigned long long num = 0;

    while (std::cin >> num)
    {
      list.push_back(num);
      isEmpty = false;
    }

    pairsList.push_back(std::make_pair(listName, list));
    std::cin.clear();
  }

  if (pairsList.empty())
  {
    std::cout << 0;
  }
  else
  {
    std::cout << pairsList.begin()->first;

    for (auto i = ++pairsList.begin(); i != pairsList.end(); ++i)
    {
      std::cout << ' ' << i->first;
    }
    std::cout << '\n';

    try
    {
      printInfo(std::cout, pairsList);
    }
    catch (const std::overflow_error &)
    {
      std::cerr << "Overflow error!\n";
      return 1;
    }
  }
  std::cout << '\n';
}
