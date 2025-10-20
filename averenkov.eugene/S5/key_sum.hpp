#include <limits>
#include <string>
#include <stdexcept>

namespace averenkov
{
  struct KeySum
  {
    void operator()(const std::pair< const long long, std::string >& key_value);
    long long sum_ = 0;
    std::string values_;
  };

  void KeySum::operator()(const std::pair< const long long, std::string >& key_value)
  {
    if (sum_ > 0 && key_value.first > std::numeric_limits< long long >::max() - sum_)
    {
      throw std::overflow_error("overflow");
    }
    if (sum_ < 0 && key_value.first < std::numeric_limits< long long >::min() - sum_)
    {
      throw std::overflow_error("overflow");
    }
    sum_ += key_value.first;
    values_ += key_value.second + " ";
  }

}
