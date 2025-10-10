#ifndef KEYSUM_HPP
#define KEYSUM_HPP

#include <stdexcept>
#include <string>
#include <limits>

namespace kushekbaev
{
  struct KeySum
  {
    KeySum operator()(const std::pair< const long long int, std::string >& keyValue);
    long long int sum = 0;
    std::string value;
  };

  KeySum KeySum::operator()(const std::pair< const long long int, std::string >& keyValue)
  {
    KeySum result;
    result.sum = this->sum;
    result.value = this->value;
    if (sum > 0 && keyValue.first > std::numeric_limits< long long int >::max() - sum)
    {
      throw std::overflow_error("Overflow!");
    }
    if (sum < 0 && keyValue.first < std::numeric_limits< long long int >::min() - sum)
    {
      throw std::underflow_error("Underflow!");
    }
    result.sum += keyValue.first;
    result.value += keyValue.second + " ";
    return result;
  }
}

#endif
