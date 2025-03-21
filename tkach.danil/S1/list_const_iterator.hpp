#ifndef LIST_CONST_ITERATOR_HPP
#define LIST_CONST_ITERATOR_HPP

#include <iterator>
#include "node.hpp"

namespace tkach
{
  template< typename T >
  class List;

  template< typename T >
  class Citerator: public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List < T >;
  public:
    using this_t = Citerator< T >;
    Citerator();
    Citerator(const this_t&) = default;
    ~Citerator() = default;
    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    const T & operator*() const;
    const T * operator->() const;
    bool operator!=(const this_t& rhs) const;
    bool operator==(const this_t& rhs) const;
  private:
    Node< T >* node_;
    explicit Citerator(Node< T >* node);
  };

  template< typename T >
  Citerator< T >::Citerator():
    node_(nullptr)
  {}

  template< typename T >
  Citerator< T >& Citerator< T >::operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  Citerator< T > Citerator< T >::operator++(int)
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T& Citerator< T >::operator*() const
  {
    return node_->data_;
  }

  template< typename T >
  const T* Citerator< T >::operator->() const
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool Citerator< T >::operator==(const this_t& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool Citerator< T >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  Citerator< T >::Citerator(Node< T >* node):
    node_(node)
  {}
}

#endif
