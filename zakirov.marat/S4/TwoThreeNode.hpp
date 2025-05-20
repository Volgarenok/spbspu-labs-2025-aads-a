#ifndef TWOTHREENODE_HPP
#define TWOTHREENODE_HPP
#include <cstddef>
#include <utility>

namespace zakirov
{
  template< typename T, typename K >
  class TwoThreeNode
  {
  public:
    TwoThreeNode(const T & data, K key);
    TwoThreeNode(const T & fdata, K fkey, const T & sdata, K skey);
    TwoThreeNode< T, K > * top_;
    TwoThreeNode< T, K > * left_;
    TwoThreeNode< T, K > * middle_;
    TwoThreeNode< T, K > * right_;
    std::pair< T, K > first_value_;
    std::pair< T, K > second_value_;
    bool second_exists_;
  };

  template< typename T, typename K >
  TwoThreeNode< T, K >::TwoThreeNode(const T & data, K key):
    top_(nullptr),
    left_(nullptr),
    middle_(nullptr),
    right_(nullptr),
    first_value_(data, key),
    second_exists_(false)
  {}

  template< typename T, typename K >
  TwoThreeNode< T, K >::TwoThreeNode(const T & fdata, K fkey, const T & sdata, K skey):
    top_(nullptr),
    left_(nullptr),
    middle_(nullptr),
    right_(nullptr),
    first_value_(fdata, fkey),
    second_value_(sdata, skey),
    second_exists_(true)
  {
    if (fkey > skey)
    {
      std::pair< T, K > buffer_pair = first_value_;
      first_value_ = second_value_;
      second_value_ = buffer_pair;
    }
  }
}

#endif
