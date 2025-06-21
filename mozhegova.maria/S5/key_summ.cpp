#include "key_summ.hpp"
#include <limits>
#include <stdexcept>

namespace
{
  void isOverflow(int op1, int op2)
  {
    const int max = std::numeric_limits< int >::max();
    const int min = std::numeric_limits< int >::min();
    if (op1 > 0 && op2 > 0 && op1 > max - op2)
    {
      throw std::overflow_error("overflow");
    }
    if (op1 < 0 && op2 < 0 && op1 < min - op2)
    {
      throw std::overflow_error("underflow");
    }
  }
}

mozhegova::KeySumm::KeySumm():
  keySum(0),
  values()
{}

void mozhegova::KeySumm::operator()(const std::pair< int, std::string > & keyValue)
{
  isOverflow(keySum, keyValue.first);
  keySum += keyValue.first;
  if (!values.empty())
  {
    values += ' ';
  }
  values += keyValue.second;
}
