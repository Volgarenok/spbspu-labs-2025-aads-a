#include <iostream>
#include "io-sequences.hpp"
#include "list-manip.hpp"

int main()
{
  using namespace sveshnikov;
  list_pair_t list;
  inputLists(std::cin, list);
  if (list.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  outputNamesLists(std::cout, list) << '\n';
  if (sveshnikov::getMaxSizeLists(list) != 0)
  {
    outputNewLists(std::cout, list) << '\n';
  }
  try
  {
    outputSumsNewLists(std::cout, list) << '\n';
  }
  catch (const std::overflow_error &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  catch (const std::bad_alloc &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}
