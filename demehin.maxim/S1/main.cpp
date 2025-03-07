#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <list/list.hpp>
#include <calc_utils.hpp>

namespace
{
  using ullList = demehin::List< unsigned long long >;
  using ListOfPairs = demehin::List< std::pair< std::string, ullList > >;

  size_t defineMaxSize(const ListOfPairs& pairsList)
  {
    size_t max_size = 0;
    for (auto it = pairsList.begin(); it != pairsList.end(); ++it)
    {
      max_size = std::max(max_size, it->second.size());
    }
    return max_size;
  }

  unsigned long long calculateSum(const ullList& nums)
  {
    unsigned long long sum = 0;
    for (auto it = nums.begin(); it != nums.end(); it++)
    {
      sum = demehin::getCheckedSum(sum, *it);
    }

    return sum;
  }

  void printLstNames(std::ostream& out, const ListOfPairs& pairsList)
  {
    auto begin = pairsList.begin();
    out << begin->first;
    for (auto it = ++begin; it != pairsList.end(); ++it)
    {
      out << " " << it->first;
    }
  }

  ullList formNumLst(ListOfPairs& pairsList)
  {
    ullList numsList;
    for (auto it = pairsList.begin(); it != pairsList.end(); it++)
    {
      if (!it->second.empty())
      {
        numsList.push_back(it->second.front());
        it->second.pop_front();
      }
    }
    return numsList;
  }

  void printNums(std::ostream& out, const ullList& numsList)
  {
    out << numsList.front();
    for (auto it = ++numsList.begin(); it != numsList.end(); it++)
    {
      out << " " << *it;
    }
  }

  void printValues(std::ostream& out, ListOfPairs& pairsList)
  {
    size_t max_size = defineMaxSize(pairsList);
    if (max_size == 0)
    {
      out << "0";
      return;
    }

    ullList sumList;

    for (size_t i = 0; i < max_size; i++)
    {
      ullList numsList = formNumLst(pairsList);
      if (!numsList.empty())
      {
        printNums(out, numsList);
        out << "\n";
        sumList.push_back(calculateSum(numsList));
      }
    }
    printNums(out, sumList);
  }

  void inputLists(std::istream& in, ListOfPairs& pairsList)
  {
    std::string nodeName;
    while (std::cin >> nodeName)
    {
      ullList numsList;
      unsigned long long num;
      while (in >> num)
      {
        numsList.push_back(num);
      }
      pairsList.push_back(std::make_pair(nodeName, numsList));
      in.clear();
    }
  }

}

int main()
{
  ListOfPairs pairsList;
  try
  {
    inputLists(std::cin, pairsList);
  }
  catch(const std::bad_alloc&)
  {
    std::cout << "error: bad_alloc\n";
    return 1;
  }

  if (pairsList.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  printLstNames(std::cout, pairsList);
  std::cout << "\n";

  try
  {
    printValues(std::cout, pairsList);
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "error: bad_alloc\n";
    return 1;
  }

  std::cout << "\n";
}
