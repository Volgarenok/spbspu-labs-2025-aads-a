#ifndef KEY_SUM_HPP
#define KEY_SUM_HPP

#include <limits>
#include <stdexcept>
#include <string>

namespace {

  template < typename KeyType >
  bool isAdditionOverflow(KeyType lhs, KeyType rhs)
  {
    constexpr KeyType max_value = std::numeric_limits< KeyType >::max();
    constexpr KeyType min_value = std::numeric_limits< KeyType >::min();
    return (((rhs > 0) && (lhs > (max_value - rhs))) || ((rhs < 0) && (lhs < (min_value - rhs))));
  }

}

namespace zholobov {

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
