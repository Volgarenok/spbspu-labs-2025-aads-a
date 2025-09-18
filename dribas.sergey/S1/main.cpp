#include <iostream>
#include "getSequance.hpp"
#include "outSequance.hpp"

int main()
{
  dribas::List< std::pair< std::string, dribas::List< unsigned long long > > > sequance;
  bool isOverflow = false;
  try {
    sequance = dribas::getSequance(std::cin);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (!sequance.empty()) {
    dribas::outSequanceName(std::cout, sequance);
    std::cout << '\n';
  }
  dribas::outSequanceNameSum(std::cout, sequance, isOverflow);
  std::cout << '\n';
  if (isOverflow) {
    std::cerr << "OverFlow!\n";
    return 1;
  }
  return 0;
}
