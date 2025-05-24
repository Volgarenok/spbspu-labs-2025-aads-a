#include <iostream>
#include "getSequance.hpp"
#include "outSequance.hpp"

int main()
{
  dribas::List< std::pair< std::string, dribas::List< unsigned long long > > > sequance;
  bool isOverflow = false;
  try {
    sequance = dribas::getSequance(std::cin, isOverflow);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (!sequance.empty()) {
    dribas::getSequanceName(std::cout, sequance);
    std::cout << '\n';
  }
  dribas::getSequanceNameSum(std::cout, sequance);
  std::cout << '\n';
  if (isOverflow) {
    std::cerr << "OverFlow!\n";
    return 1;
  }
  return 0;
}
