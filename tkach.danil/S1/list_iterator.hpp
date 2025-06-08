#ifndef LIST_ITERATOR_HPP
#define LIST_ITERATOR_HPP

#include <iterator>
#include "node.hpp"

namespace tkach
{
  template< typename T >
  class List;

  template< typename T >
  class Iterator: public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List < T >;
  public:
    using this_t = Iterator< T >;
    Iterator();
    Iterator(const this_t&) = default;
    ~Iterator() = default;
    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    T & operator*();
    const T & operator*() const;
    T * operator->();
    const T * operator->() const;
    bool operator!=(const this_t& rhs) const;
    bool operator==(const this_t& rhs) const;
  private:
    Node< T >* node_;
    explicit Iterator(Node< T >* node);
  };

  template< typename T >
  Iterator< T >::Iterator():
    node_(nullptr)
  {}

  template< typename T >
  Iterator< T >& Iterator< T >::operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T& Iterator< T >::operator*() const
  {
    return node_->data_;
  }

  template< typename T >
  T& Iterator< T >::operator*()
  {
    return const_cast< T& >(static_cast< const Iterator< T >* >(this)->operator*());
  }

  template< typename T >
  const T* Iterator< T >::operator->() const
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  T* Iterator< T >::operator->()
  {
    return const_cast< T* >(static_cast< const Iterator< T >* >(this)->operator->());
  }

  template< typename T >
  bool Iterator< T >::operator==(const this_t& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  Iterator< T >::Iterator(Node< T >* node):
    node_(node)
  {}
}

#endif
