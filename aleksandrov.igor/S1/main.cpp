#include <iostream>
#include <string>
#include "list.hpp"
#include "list-utils.hpp"

int main()
{
  using namespace aleksandrov;

  List< std::pair< std::string, List< unsigned long long > > > list;
  try
  {
    getPairsList(std::cin, list);
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  List< std::string > nameList;
  try
  {
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      nameList.pushBack(it->first);
    }
  }
  catch (const std::bad_alloc&)
  {
    nameList.clear();
  }
  if (nameList.empty())
  {
    std::cout << "0" << "\n";
    return 0;
  }
  std::cout << nameList.front();
  for (auto it = ++nameList.begin(); it != nameList.end(); ++it)
  {
    std::cout << " " << *it;
  }
  std::cout << "\n";

  List< List< unsigned long long > > transposedList;
  try
  {
    getTransposedList(list, transposedList);
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  if (transposedList.empty())
  {
    std::cout << "0" << "\n";
    return 0;
  }
  for (auto it = transposedList.begin(); it != transposedList.end(); ++it)
  {
    outputList(*it, std::cout);
    std::cout << "\n";
  }

  List< unsigned long long > sumList;
  for (auto it = transposedList.begin(); it != transposedList.end(); ++it)
  {
    try
    {
      unsigned long long sum = calcSum(*it);
      sumList.pushBack(sum);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }
  outputList(sumList, std::cout);
  std::cout << "\n";
}

