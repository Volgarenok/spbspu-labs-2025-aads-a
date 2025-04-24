#ifndef ACCESS_HPP
#define ACCESS_HPP

#include "declaration.hpp"

template< class T >
bool rychkov::List< T >::empty() const noexcept
{
  return size_ == 0;
}
template< class T >
typename rychkov::List< T >::size_type rychkov::List< T >::size() const noexcept
{
  return size_;
}
template< class T >
typename rychkov::List< T >::reference rychkov::List< T >::front()
{
  return head_->data;
}
template< class T >
typename rychkov::List< T >::const_reference rychkov::List< T >::front() const
{
  return head_->data;
}
template< class T >
typename rychkov::List< T >::reference rychkov::List< T >::back()
{
  return tail_->data;
}
template< class T >
typename rychkov::List< T >::const_reference rychkov::List< T >::back() const
{
  return tail_->data;
}


template< class T >
typename rychkov::List< T >::iterator rychkov::List< T >::begin() noexcept
{
  return {head_, tail_};
}
template< class T >
typename rychkov::List< T >::const_iterator rychkov::List< T >::begin() const noexcept
{
  return {head_, tail_};
}
template< class T >
typename rychkov::List< T >::const_iterator rychkov::List< T >::cbegin() const noexcept
{
  return {head_, tail_};
}
template< class T >
typename rychkov::List< T >::reverse_iterator rychkov::List< T >::rbegin() noexcept
{
  return {tail_, head_};
}
template< class T >
typename rychkov::List< T >::const_reverse_iterator rychkov::List< T >::rbegin() const noexcept
{
  return {tail_, head_};
}
template< class T >
typename rychkov::List< T >::const_reverse_iterator rychkov::List< T >::crbegin() const noexcept
{
  return {tail_, head_};
}

template< class T >
typename rychkov::List< T >::iterator rychkov::List< T >::end() noexcept
{
  return {nullptr, tail_};
}
template< class T >
typename rychkov::List< T >::const_iterator rychkov::List< T >::end() const noexcept
{
  return {nullptr, tail_};
}
template< class T >
typename rychkov::List< T >::const_iterator rychkov::List< T >::cend() const noexcept
{
  return {nullptr, tail_};
}
template< class T >
typename rychkov::List< T >::reverse_iterator rychkov::List< T >::rend() noexcept
{
  return {nullptr, head_};
}
template< class T >
typename rychkov::List< T >::const_reverse_iterator rychkov::List< T >::rend() const noexcept
{
  return {nullptr, head_};
}
template< class T >
typename rychkov::List< T >::const_reverse_iterator rychkov::List< T >::crend() const noexcept
{
  return {nullptr, head_};
}

#endif
