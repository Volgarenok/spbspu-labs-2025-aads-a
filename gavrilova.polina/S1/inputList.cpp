#include "inputList.hpp"

gavrilova::FwdList< unsigned long long > gavrilova::inputNumbers(std::istream& in)
{
  FwdList< unsigned long long > numbers;
  unsigned long long num = 0;
  auto it = numbers.cbegin();
  while (in >> num) {
    numbers.insert(it, num);
    ++it;
  }
  in.clear();
  return numbers;
}
