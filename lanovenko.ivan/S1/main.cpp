#include <iostream>
#include <utility>
#include <limits>
#include <iomanip>
#include "list_manipulations.hpp"
#include "forward_list.hpp"

int main()
{
  using namespace lanovenko;
  list_of_pairs myList;
  std::string sequenceName = "";
  while (std::cin >> sequenceName)
  {
    unsigned long long element = 0;
    list_ull temporaryList{};
    while (std::cin >> element)
    {
      temporaryList.push_back(element);
    }
    std::pair< std::string, ForwardList< unsigned long long > > temporaryPair{ sequenceName, temporaryList };
    myList.push_back(temporaryPair);
    if (std::cin.eof())
    {
      break;
    }
    std::cin.clear();
  }
  printSequencesName(myList, std::cout);
  size_t maxSize = 0;
  for (const auto& it: myList)
  {
    if (maxSize < it.second.size())
    {
      maxSize = it.second.size();
    }
  }
  bool sumLimit = false;
  printSequences(myList, maxSize, sumLimit, std::cout);
  if (sumLimit)
  {
    std::cerr << "Sum limit!\n";
    return 1;
  }
}
