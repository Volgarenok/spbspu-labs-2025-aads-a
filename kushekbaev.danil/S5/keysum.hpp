#ifndef KEYSUM_HPP
#define KEYSUM_HPP

#include <stdexcept>
#include <string>
#include <limits>

namespace kushekbaev
{
  struct KeySum
  {
    void operator()(const std::pair< const long long int, std::string >& keyValue);
    long long int sum = 0;
    std::string value = "";
  };

  void KeySum::operator()(const std::pair< const long long int, std::string >& keyValue)
  {
    if (sum > 0 && keyValue.first > std::numeric_limits< long long int >::max() - sum)
    {
      throw std::overflow_error("Overflow!\n");
    }
    if (sum < 0 && keyValue.first < std::numeric_limits< long long int >::min() - sum)
    {
      throw std::underflow_error("Underflow!\n");
    }
    sum += keyValue.first;
    value += keyValue.second + " ";
  }
}

#endif
