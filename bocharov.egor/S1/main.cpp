#include <exception>
#include <iostream>
#include "actions.hpp"

using namespace bocharov;

int main()
{
  try
  {
    pairs_list_t pairs_list = createList(std::cin);
    if (pairs_list.empty())
    {
      std::cout << 0 << '\n';
      return 0;
    }
    outputList(std::cout, pairs_list);
    print_lists_info(std::cout, pairs_list);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  std::cout << '\n';
}
