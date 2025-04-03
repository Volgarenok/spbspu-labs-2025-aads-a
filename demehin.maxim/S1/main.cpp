#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <list/list.hpp>
#include <calc_utils.hpp>

namespace
{
  using ListOfUll = demehin::List< unsigned long long >;
  using ListOfPairs = demehin::List< std::pair< std::string, ListOfUll > >;

  size_t defineMaxSize(const ListOfPairs& pairsList)
  {
    if (pairsList.empty())
    {
      return 0;
    }

    size_t max_size = 0;
    for (auto it = pairsList.begin(); it != pairsList.end(); ++it)
    {
      max_size = std::max(max_size, it->second.size());
    }
    return max_size;
  }

  unsigned long long calculateSum(const ListOfUll& nums)
  {
    unsigned long long sum = 0;
    for (auto it = nums.begin(); it != nums.end(); it++)
    {
      sum = demehin::sumChecked(sum, *it);
    }

    return sum;
  }

  void printLstNames(std::ostream& out, const ListOfPairs& pairsList)
  {
    if (pairsList.empty())
    {
      return;
    }

    auto begin = pairsList.begin();
    out << begin->first;
    for (auto it = ++begin; it != pairsList.end(); ++it)
    {
      out << " " << it->first;
    }
  }

  ListOfUll formNumLst(const ListOfPairs& pairsList)
  {
    ListOfUll numsList;
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

  void printNums(std::ostream& out, const ListOfUll& numsList)
  {
    out << numsList.front();
    for (auto it = ++numsList.begin(); it != numsList.end(); it++)
    {
      out << " " << *it;
    }
  }

  void formAndPrintLine(std::ostream& out, const ListOfPairs& lst, ListOfUll& sumList)
  {
    ListOfUll numsList = formNumLst(lst);
    if (!numsList.empty())
    {
      printNums(out, numsList);
      out << "\n";
      sumList.push_back(calculateSum(numsList));
    }
  }

  void printListsInfo(std::ostream& out, const ListOfPairs& pairsList)
  {
    ListOfPairs lstCpy(pairsList);

    size_t max_size = defineMaxSize(lstCpy);
    if (max_size == 0)
    {
      if (!pairsList.empty())
      {
        printLstNames(out, lstCpy);
        out << "\n";
      }
      out << "0";
      return;
    }

    printLstNames(out, lstCpy);
    out << "\n";

    ListOfUll sumList;
    for (size_t i = 0; i < max_size; i++)
    {
      formAndPrintLine(out, lstCpy, sumList);
    }
    printNums(out, sumList);
  }

  void inputLists(std::istream& in, ListOfPairs& pairsList)
  {
    std::string nodeName;
    while (std::cin >> nodeName)
    {
      ListOfUll numsList;
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
    printListsInfo(std::cout, pairsList);
    std::cout << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
