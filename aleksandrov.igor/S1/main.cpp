#include <iostream>
#include <string>
#include "list-utils.hpp"
#include "list.hpp"

int main()
{
  using namespace aleksandrov;

  PairsList list;
  List< std::string > nameList;
  try
  {
    getPairsList(std::cin, list);
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      nameList.emplaceBack(it->first);
    }
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "ERROR: Out of memory!" << '\n';
    return 1;
  }
  if (nameList.empty())
  {
    std::cout << '0' << '\n';
    return 0;
  }
  std::cout << nameList.front();
  for (auto it = ++nameList.cbegin(); it != nameList.cend(); ++it)
  {
    std::cout << ' ' << *it;
  }
  std::cout << '\n';

  List< List< unsigned long long > > transposedList;
  try
  {
    getTransposedList(list, transposedList);
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "ERROR: Out of memory!" << '\n';
    return 1;
  }
  if (transposedList.empty())
  {
    std::cout << '0' << '\n';
    return 0;
  }
  for (auto it = transposedList.cbegin(); it != transposedList.cend(); ++it)
  {
    outputList(*it, std::cout);
    std::cout << '\n';
  }

  List< unsigned long long > sumList;
  for (auto it = transposedList.begin(); it != transposedList.end(); ++it)
  {
    try
    {
      unsigned long long sum = calcSum(*it);
      sumList.emplaceBack(sum);
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "ERROR: Out of memory!" << '\n';
      return 1;
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  outputList(sumList, std::cout);
  std::cout << '\n';
}

