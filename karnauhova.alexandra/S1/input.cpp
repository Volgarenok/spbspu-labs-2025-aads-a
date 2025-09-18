#include "input.hpp"
#include <iostream>
#include <string>
#include <stdexcept>


std::pair< std::string, karnauhova::FwdList< unsigned long long > > karnauhova::input_line(std::istream& in, std::string name)
{
  unsigned long long x = 0;
  FwdList< unsigned long long > numbers;
  while (in >> x)
  {
    if (!x)
    {
      throw std::logic_error("Incorrect number");
    }
    numbers.push_front(x);
  }
  in.clear();
  numbers.reverse();
  std::pair< std::string, karnauhova::FwdList< unsigned long long > > p (name, numbers);
  return p;
}
