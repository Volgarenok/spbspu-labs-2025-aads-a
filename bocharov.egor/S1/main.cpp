#include <iostream>
#include "actions.hpp"
#include <exception>

int main()
{
  try
  {
    bocharov::pairs_list_t pairs_list = bocharov::createList(std::cin);
    if (pairs_list.empty())
    {
      std::cout << 0 << '\n';
      return 0;
    }
    bocharov::outputList(std::cout, pairs_list);
    bocharov::print_lists_info(std::cout, pairs_list);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  std::cout << '\n';
}
