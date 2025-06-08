#include <iostream>
#include <limits>
#include "inputProcess.hpp"
#include <list/list.hpp>
#include <list/iterators.hpp>

int main()
{
  using namespace alymova;
  try
  {
    list_pair_t list;
    inputProcess(std::cin, list);
    if (list.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    outputListString(std::cout, list);
    std::cout << "\n";
    size_t max_size = findMaxListSize(list);
    if (max_size == 0)
    {
      std::cout << "0\n";
      return 0;
    }
    for (size_t i = 0; i < max_size; ++i)
    {
      outputProcessOne(std::cout, list, i);
      std::cout << "\n";
    }
    list_int_t sums = countSums(list);
    outputListInt(std::cout, sums);
    std::cout << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
