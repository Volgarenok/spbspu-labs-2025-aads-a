#include <exception>
#include "actionsList.hpp"
int main()
{
  try
  {
    kiselev::list listPairs = kiselev::createList(std::cin);
    if (listPairs.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    outputName(std::cout, listPairs) << "\n";
    if (listPairs.front().second.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    outputNumbers(std::cout, listPairs);
    kiselev::numberList sum = calculateSum(listPairs);
    outputNumber(std::cout, sum) << "\n";
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
