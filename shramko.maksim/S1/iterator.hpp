#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "FwdListNode.hpp"

namespace shramko
{
  template< typename T >
  class ForwardList;

  template< typename T >
  class Iterator : public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using SelfType = Iterator< T >;

    Iterator() : node_(nullptr), isFirstPass_(true) {}
    Iterator(ListNode<T>* node): node_(node), isFirstPass_(true) {}

    SelfType& operator++()
    {
      node_ = node_->nextPtr;
      isFirstPass_ = false;
      return *this;
    }

    SelfType operator++(int)
    {
      SelfType temp = *this;
      ++(*this);
      return temp;
    }

    T& operator*() { return node_->dataValue; }
    T* operator->() { return &node_->dataValue; }
    const T& operator*() const { return node_->dataValue; }
    const T* operator->() const { return &node_->dataValue; }

    bool operator==(const SelfType& other) const
    {
      return node_ == other.node_ && isFirstPass_ == other.isFirstPass_;
    }

    bool operator!=(const SelfType& other) const { return !(*this == other); }

  private:
    ListNode<T>* node_;
    bool isFirstPass_;
  };
}

#endif
