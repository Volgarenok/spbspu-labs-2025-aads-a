#include "sumKey.hpp"
#include <limits>
#include <stdexcept>

void dribas::SumKey::operator()(const std::pair< const long long, std::string >& keyValue)
{
  if (key > 0 && keyValue.first > std::numeric_limits< long long >::max() - key) {
    throw std::overflow_error("overflow");
  }
  if (key < 0 && keyValue.first < std::numeric_limits< long long >::min() - key) {
    throw std::overflow_error("overflow");
  }

  key += keyValue.first;
  if (value.empty()) {
    value = keyValue.second;
  } else {
    value.reserve(value.size() + 1 + keyValue.second.size());
    value += ' ';
    value += keyValue.second;
  }
}
