#ifndef CONSTRUCT_DESTRUCT_HPP
#define CONSTRUCT_DESTRUCT_HPP

#include "declaration.hpp"

#include <utility>

template< class T >
rychkov::List< T >::List() noexcept:
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{}
template< class T >
rychkov::List< T >::~List()
{
  clear();
}
template< class T >
void rychkov::List< T >::clear() noexcept
{
  erase(begin(), end());
}

template< class T >
rychkov::List< T >::List(const List& rhs):
  List(rhs.begin(), rhs.end())
{}
template< class T >
rychkov::List< T >::List(List&& rhs) noexcept:
  head_(std::exchange(rhs.head_, nullptr)),
  tail_(std::exchange(rhs.tail_, nullptr)),
  size_(std::exchange(rhs.size_, 0))
{}

template< class T >
rychkov::List< T >::List(value_type value, size_type count):
  List()
{
  if (count == 0)
  {
    return;
  }
  size_ = count;
  tail_ = new node_t< value_type >{value};
  head_ = tail_;
  for (size_type i = 1; i < count; ++i)
  {
    tail_->next = new node_t< value_type >{value, tail_};
    tail_ = tail_->next;
  }
}
template< class T >
template< class InputIt >
rychkov::List< T >::List(InputIt from, InputIt to):
  List()
{
  if (from == to)
  {
    return;
  }
  tail_ = new node_t< value_type >{*from};
  head_ = tail_;
  ++size_;
  ++from;
  for (; from != to; ++from)
  {
    tail_->next = new node_t< value_type >{*from, tail_};
    tail_ = tail_->next;
    size_++;
  }
}
template< class T >
rychkov::List< T >::List(std::initializer_list< value_type > rhs):
  List(rhs.begin(), rhs.end())
{}
template< class T >
rychkov::List< T >& rychkov::List< T >::operator=(const List& rhs)
{
  List< T > temp(rhs);
  swap(temp);
  return *this;
}
template< class T >
rychkov::List< T >& rychkov::List< T >::operator=(List&& rhs) noexcept
{
  List< T > temp(std::move(rhs));
  swap(temp);
  return *this;
}
template< class T >
rychkov::List< T >& rychkov::List< T >::operator=(std::initializer_list< value_type > rhs)
{
  List< T > temp(std::move(rhs));
  swap(temp);
  return *this;
}


#endif
