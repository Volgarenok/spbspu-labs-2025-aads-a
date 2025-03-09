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
    alymova::inputProcess(std::cin, list);
    if (list.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    alymova::outputListString(std::cout, list);
    std::cout << "\n";
    alymova::outputProcess(std::cout, list);
    list_int_t sums = countSums(list);
    if (sums.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    alymova::outputListInt(std::cout, sums);
    std::cout << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
