#include <iostream>
#include <list>
#include <sstream>
#include <cctype>
#include "inputProcess.hpp"
#include "list.hpp"
#include "iterators.hpp"

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
    outputProcess(std::cout, list);
    list_int_t sums = countSums(list);
    if (sums.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    outputListInt(std::cout, sums);
    std::cout << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
