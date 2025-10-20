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

sharifullina::Collector::Collector():
  collector(0),
  values()
{}

void sharifullina::Collector::operator()(const std::pair< int, std::string > & keyValue)
{
  checkOverflow(collector, keyValue.first);
  collector += keyValue.first;
  if (!values.empty())
  {
    values += ' ';
  }
  values += keyValue.second;
}
