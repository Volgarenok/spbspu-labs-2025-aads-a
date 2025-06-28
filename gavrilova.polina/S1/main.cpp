#include <iostream>
#include <string>
#include "FwdList.hpp"
#include "inputList.hpp"
#include "outputList.hpp"

int main()
{
  using namespace gavrilova;
  using ULL = unsigned long long;

  FwdList< std::pair< std::string, FwdList< ULL > > > listOfPairs{};
  size_t numOfPairs = 0;
  std::string name;
  size_t maxLen = 0;
  auto cur_it = listOfPairs.begin();
  while (std::cin >> name && !std::cin.eof()) {
    FwdList< ULL > numbers = inputNumbers(std::cin);
    maxLen = (maxLen < numbers.size()) ? numbers.size() : maxLen;
    std::pair< std::string, FwdList< ULL > > new_pair{name, numbers};
    listOfPairs.insert(cur_it, new_pair);
    ++cur_it;
    ++numOfPairs;
  }

  if (numOfPairs) {
    outNames(std::cout, listOfPairs);
    std::cout << "\n";
  }

  if (maxLen == 0) {
    std::cout << "0" << "\n";
    return 0;
  }

  FwdList< ULL > sums{};
  try {
    sums = outNumbers(std::cout, listOfPairs, maxLen, numOfPairs);
    std::cout << "\n";
  } catch (const std::overflow_error& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  outFwdListULL(std::cout, sums);
  std::cout << "\n";
}
