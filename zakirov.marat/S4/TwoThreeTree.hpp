#ifndef TWOTHREETREE_HPP
#define TWOTHREETREE_HPP
#include "TwoThreeNode.hpp"

namespace zakirov
{
  template < typename T, typename K >
  class TwoThreeTree
  {
  public:

  private:
    TwoThreeNode< T, K > * head_;
    size_t size_;
  };
}

#endif
