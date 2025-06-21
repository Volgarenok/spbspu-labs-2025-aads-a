#include "functionalObject.hpp"
#include <limits>
#include <stdexcept>

namespace
{
  void checkOverflow(int f, int s)
  {
    const int max = std::numeric_limits< int >::max();
    const int min = std::numeric_limits< int >::min();
    if ((s > 0) && (f > max - s))
    {
      throw std::overflow_error("ERROR: overflow");
    }
    if ((s < 0) && (f < min - s))
    {
      throw std::overflow_error("ERROR: underflow");
    }
  }
}

finaev::keySum::keySum():
  key_sum(0),
  values()
{}

void finaev::keySum::operator()(const std::pair< int, std::string > & keyValue)
{
  checkOverflow(key_sum, keyValue.first);
  key_sum += keyValue.first;
  if (!values.empty())
  {
    values += ' ';
  }
  values += keyValue.second;
}
