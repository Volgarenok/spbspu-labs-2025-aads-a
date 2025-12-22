#include <algorithm>
#include <exception>
#include <iostream>
#include <cstdint>
#include <limits>
#include <string>
#include "list.hpp"
#include "funtions.hpp"

int main()
{
  List< NamedList > data;

  inputData(data, std::cin);
  if (data.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  printDataNames(data);
  printDataValues(data);

  List< uint64_t > sums;
  try
  {
    countSums(data, sums);
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << "\n";
    return 1;
  }
  printList(sums.begin(), sums.end());

  return 0;
}
