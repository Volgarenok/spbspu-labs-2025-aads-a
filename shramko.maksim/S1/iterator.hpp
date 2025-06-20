#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "FwdListNode.hpp"

namespace shramko
{
  template< typename T >
  class ForwardList;

  template< typename T >
  class Iterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator():
      node_(nullptr),
      isFirstPass_(true)
    {}

    explicit Iterator(ListNode< T >* node):
      node_(node),
      isFirstPass_(true)
    {}

    Iterator& operator++()
    {
      node_ = node_->nextPtr;
      isFirstPass_ = false;
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    reference operator*()
    {
      return node_->dataValue;
    }

    pointer operator->()
    {
      return &node_->dataValue;
    }

    const T& operator*() const
    {
      return node_->dataValue;
    }

    const T* operator->() const
    {
      return &node_->dataValue;
    }

    bool operator==(const Iterator& other) const
    {
      return node_ == other.node_ && isFirstPass_ == other.isFirstPass_;
    }

    bool operator!=(const Iterator& other) const
    {
      return !(*this == other);
    }

  private:
    friend class ForwardList< T >;
    ListNode< T >* node_;
    bool isFirstPass_;
  };
}

#endif
