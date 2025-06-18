#include <iostream>
#include <string>
#include "list.hpp"
#include "input_outputLists.hpp"
#include "outputSums.hpp"

int main()
{
  using namespace abramov;
  List< std::pair< std::string, List< unsigned long long int > > > lists;
  size_t count = 0;
  size_t lists_count = 0;
  inputLists(std::cin, &lists, lists_count, count);
  if (!lists_count)
  {
    std::cout << "0\n";
    return 0;
  }
  printNames(std::cout, &lists);
  printValues(std::cout, &lists, count);
  try
  {
    outputSums(std::cout, &lists, count);
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "Memory fail\n";
    return 2;
  }
  catch (const std::logic_error &e)
  {
    std::cerr << e.what();
    return 1;
  }
}
