#include "accumulator.hpp"
#include <limits>
#include <stdexcept>

namespace
{
  void checkOverflow(int op1, int op2)
  {
    const int max = std::numeric_limits< int >::max();
    const int min = std::numeric_limits< int >::min();
    if ((op2 > 0) && (op1 > max - op2))
    {
      throw std::runtime_error("ERROR: overflow");
    }
    if ((op2 < 0) && (op1 < min - op2))
    {
      throw std::runtime_error("ERROR: underflow");
    }
  }
}

lanovenko::KeyValueAccumulator::KeyValueAccumulator():
  keySum(0),
  values()
{}

void lanovenko::KeyValueAccumulator::operator()(const std::pair< int, std::string > & keyValue)
{
  checkOverflow(keySum, keyValue.first);
  keySum += keyValue.first;
  if (!values.empty())
  {
    values += ' ';
  }
  values += keyValue.second;
}
