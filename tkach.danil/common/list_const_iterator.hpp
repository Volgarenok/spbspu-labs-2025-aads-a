#ifndef LIST_CONST_ITERATOR_HPP
#define LIST_CONST_ITERATOR_HPP

#include <iterator>
#include "node.hpp"

namespace tkach
{
  template< typename T >
  class List;

  template< typename T >
  class CListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List < T >;
  public:
    using this_t = CListIterator< T >;
    CListIterator();
    CListIterator(const this_t&) = default;
    ~CListIterator() = default;
    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    const T & operator*() const;
    const T * operator->() const;
    bool operator!=(const this_t& rhs) const;
    bool operator==(const this_t& rhs) const;
  private:
    Node< T >* node_;
    Node< T >* head_node_ptr_;
    bool is_past_the_end_;
    explicit CListIterator(Node< T >* node, Node< T >* head_ptr, bool is_end_state);
  };

  template< typename T >
  CListIterator< T >::CListIterator():
    node_(nullptr),
    head_node_ptr_(nullptr),
    is_past_the_end_(true)
  {}

  template< typename T >
  CListIterator< T >& CListIterator< T >::operator++()
  {
    if (is_past_the_end_)
    {
      return *this;
    }
    if (node_ == nullptr)
    {
      is_past_the_end_ = true;
      return *this;
    }
    node_ = node_->next_;
    if (node_ == head_node_ptr_)
    {
      is_past_the_end_ = true;
    }
    return *this;
  }

  template< typename T >
  CListIterator< T > CListIterator< T >::operator++(int)
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T& CListIterator< T >::operator*() const
  {
    return node_->data_;
  }

  template< typename T >
  const T* CListIterator< T >::operator->() const
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool CListIterator< T >::operator==(const this_t& rhs) const
  {
    if (is_past_the_end_ && rhs.is_past_the_end_)
    {
        return true;
    }
    if (is_past_the_end_ != rhs.is_past_the_end_)
    {
        return false;
    }
    return node_ == rhs.node_;
  }

  template< typename T >
  bool CListIterator< T >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  CListIterator< T >::CListIterator(Node< T >* node, Node< T >* head_ptr, bool is_end_state):
    node_(node),
    head_node_ptr_(head_ptr),
    is_past_the_end_(is_end_state)
  {}
}

#endif
