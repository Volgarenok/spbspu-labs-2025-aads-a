#include "getSequance.hpp"
#include <cstddef>
#include <iostream>
#include <limits>
#include "list.hpp"
#include "iterator.hpp"

dribas::List< std::pair< std::string, dribas::List< unsigned long long > > >dribas::getSequance(std::istream& input)
{
  List< std::pair< std::string, List< unsigned long long > > >allSequance;
  std::string sequance;
  while (input >> sequance) {
    List< unsigned long long > numbers{};
    unsigned long long number = 0;
    while (input >> number) {
      if (!input) {
        throw std::invalid_argument("uncorrect numbers input");
      }
      numbers.push_back(number);
    }
    allSequance.push_back(std::make_pair(sequance, numbers));
    input.clear();
  }
  return allSequance;
}
