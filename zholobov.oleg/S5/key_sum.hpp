#ifndef KEY_SUM_HPP
#define KEY_SUM_HPP

#include <limits>
#include <stdexcept>
#include <string>

namespace zholobov {

  template < typename KeyType >
  bool isAdditionOverflow(KeyType lhs, KeyType rhs)
  {
    static_assert(std::is_integral< KeyType >::value && std::is_signed< KeyType >::value,
        "KeyType must be a signed integral type.");
    constexpr KeyType max_value = std::numeric_limits< KeyType >::max();
    constexpr KeyType min_value = std::numeric_limits< KeyType >::min();
    return (((rhs > 0) && (lhs > (max_value - rhs))) || ((rhs < 0) && (lhs < (min_value - rhs))));
  }

  template < typename KeyType >
  struct KeySum {
    void operator()(const std::pair< const KeyType, std::string >& key_value);
    KeyType sum_ = 0;
    std::string values_;
  };

  template < typename KeyType >
  void KeySum< KeyType >::operator()(const std::pair< const KeyType, std::string >& key_value)
  {
    if (isAdditionOverflow(sum_, key_value.first)) {
      throw std::overflow_error("Numeric overflow");
    }
    sum_ += key_value.first;
    values_ += (" " + key_value.second);
  }

}

#endif
