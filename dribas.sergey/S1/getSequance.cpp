#include "getSequance.hpp"
#include <cstddef>
#include <iostream>
#include <limits>
#include "list.hpp"
#include "iterator.hpp"

dribas::List< std::pair< std::string, dribas::List< unsigned long long > > >dribas::getSequance(std::istream& input, bool& overflow)
{
  List< std::pair< std::string, List< unsigned long long > > >allSequance;
  std::string sequance;
  while (input >> sequance) {
    List< unsigned long long > numbers{};
    unsigned long long number = 0;
    while (input >> number) {
      if (number > static_cast< unsigned long long >(std::numeric_limits< int >::max())) {
        overflow = 1;
      }
      numbers.push_back(number);
    }
    allSequance.push_back(std::make_pair(sequance, numbers));
    input.clear();
  }
  return allSequance;
}
