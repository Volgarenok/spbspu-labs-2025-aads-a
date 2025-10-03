#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <iterator>

#include "FwdListNode.hpp"

namespace shramko
{
  template< typename T >
  class ForwardList;

  template< typename T >
  class ConstIterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator():
      node_(nullptr),
      isAtBegin_(true)
    {}

    explicit ConstIterator(ListNode< T >* node):
      node_(node),
      isAtBegin_(true)
    {}

    ConstIterator& operator++()
    {
      node_ = node_->nextPtr;
      isAtBegin_ = false;
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator temp = *this;
      ++(*this);
      return temp;
    }

    reference operator*() const
    {
      return node_->dataValue;
    }

    pointer operator->() const
    {
      return &node_->dataValue;
    }

    bool operator==(const ConstIterator& other) const
    {
      return node_ == other.node_ && isAtBegin_ == other.isAtBegin_;
    }

    bool operator!=(const ConstIterator& other) const
    {
      return !(*this == other);
    }

  private:
    friend class ForwardList< T >;
    ListNode< T >* node_;
    bool isAtBegin_;
  };
}

#endif
