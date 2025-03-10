#include <exception>
#include "actionsList.hpp"
int main()
{
  try
  {
    kiselev::list listPairs = kiselev::createList(std::cin);
    kiselev::output(std::cout, listPairs);
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
