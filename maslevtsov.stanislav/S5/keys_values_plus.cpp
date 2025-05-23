#include "keys_values_plus.hpp"
#include <limits>
#include <stdexcept>

namespace {
  int checked_addition(int a, int b)
  {
    constexpr int max_int = std::numeric_limits< int >::max();
    constexpr int min_int = std::numeric_limits< int >::min();
    if ((a > 0 && b > 0 && a > max_int - b) || (a < 0 && b < 0 && a < min_int - b)) {
      throw std::overflow_error("overflow error");
    }
    return a + b;
  }
}

void maslevtsov::KeysValuesPlus::operator()(const std::pair< int, std::string >& value)
{
  keys_sum = checked_addition(keys_sum, value.first);
  values += ' ' + value.second;
}
