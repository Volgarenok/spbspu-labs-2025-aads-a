#include <iostream>
#include <string>
#include "actionsWithList.hpp"
#include "list.hpp"

int main()
{
  lebedev::List< std::pair< std::string, lebedev::List< size_t > > > sequences;
  std::string name;

  while (std::cin >> name)
  {
    lebedev::List< size_t > numbers;
    size_t num = 0;
    while (std::cin >> num)
    {
      numbers.push_back(num);
    }
    sequences.push_back(std::make_pair(name, numbers));
    if (std::cin.eof())
    {
      break;
    }
    std::cin.clear();
  }

  lebedev::List< std::string > names;
  for (auto seq : sequences)
  {
    names.push_back(seq.first);
  }
  lebedev::printList(names, std::cout);

  bool allEmpty = true;
  for (auto seq : sequences)
  {
    if (!seq.second.empty())
    {
      allEmpty = false;
      break;
    }
  }
  if (allEmpty)
  {
    std::cout << "0\n";
    return 0;
  }

  lebedev::List< lebedev::List< size_t > > reorderedList = lebedev::createReorderedList(sequences);
  for (const auto& seq : reorderedList)
  {
    lebedev::printList(seq, std::cout);
  }
  std::pair< lebedev::List< size_t >, bool > listOfSum = lebedev::createListOfSum(reorderedList);
  if (listOfSum.second)
  {
    std::cerr << "Is Overflow\n";
    return 1;
  }
  lebedev::printList(listOfSum.first, std::cout);
}
