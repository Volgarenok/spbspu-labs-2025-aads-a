#include "KeyValue.hpp"
#include <limits>
#include <stdexcept>

void duhanina::KeySum::operator()(const std::pair< const long long, std::string >& key_value)
{
  if (key > 0 && key_value.first > std::numeric_limits< long long >::max() - key)
  {
    throw std::overflow_error("Key sum overflow");
  }
  if (key < 0 && key_value.first < std::numeric_limits< long long >::min() - key)
  {
    throw std::overflow_error("Key sum underflow");
  }
  key += key_value.first;
  if (!value.empty())
  {
    value += " ";
  }
  value += key_value.second;
}
