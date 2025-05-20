#include <exception>
#include "listact.hpp"
int main()
{
  try
  {
    brevnov::list listpair = brevnov::createList(std::cin);
    if (listpair.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    brevnov::outputName(std::cout, listpair) << "\n";
    if (listpair.front().second.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    brevnov::outputNumbers(std::cout, listpair);
    brevnov::numbers sum = brevnov::calculateSum(listpair);
    brevnov::outputNumber(std::cout, sum) << "\n";
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
