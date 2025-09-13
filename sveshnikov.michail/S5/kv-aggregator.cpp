#include "kv-aggregator.hpp"
#include <stdexcept>
#include <limits>

void sveshnikov::KeyValAggr::operator()(const std::pair< int, std::string > &key_value)
{
  constexpr int max_int = std::numeric_limits< int >::max();
  constexpr int min_int = std::numeric_limits< int >::min();
  if (key_value.first > 0 && key_sum_ > max_int - key_value.first)
  {
    throw std::overflow_error("ERROR: Overflow when calculating the amount of keys!");
  }
  if (key_value.first < 0 && key_sum_ < min_int - key_value.first)
  {
    throw std::underflow_error("ERROR: Underflow when calculating the amount of keys!");
  }
  key_sum_ += key_value.first;
  values_ = values_.empty() ? key_value.second : (values_ + " " + key_value.second);
}
