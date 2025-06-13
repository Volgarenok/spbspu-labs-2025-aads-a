#ifndef REST_HPP
#define REST_HPP

#include "declaration.hpp"

#include <utility>
#include <functional>

template< class T >
void rychkov::List< T >::swap(List& rhs) noexcept
{
  std::swap(head_, rhs.head_);
  std::swap(tail_, rhs.tail_);
  std::swap(size_, rhs.size_);
}
template< class T >
void rychkov::List< T >::reverse() noexcept
{
  const_iterator i = begin();
  while (i != end())
  {
    const_iterator temp = i;
    ++i;
    std::swap(temp.node_->prev, temp.node_->next);
  }
  std::swap(head_, tail_);
}

template< class T >
bool rychkov::List< T >::operator==(const List& rhs) const
{
  if (size_ != rhs.size_)
  {
    return false;
  }
  return compare(rhs, std::equal_to<>());
}
template< class T >
bool rychkov::List< T >::operator!=(const List& rhs) const
{
  return !(*this == rhs);
}
template< class T >
bool rychkov::List< T >::operator<=(const List& rhs) const
{
  return compare(rhs, std::less_equal<>());
}
template< class T >
bool rychkov::List< T >::operator>=(const List& rhs) const
{
  return compare(rhs, std::greater_equal<>());
}
template< class T >
bool rychkov::List< T >::operator<(const List& rhs) const
{
  return compare(rhs, std::less<>());
}
template< class T >
bool rychkov::List< T >::operator>(const List& rhs) const
{
  return compare(rhs, std::greater<>());
}

template< class T >
template< class C >
bool rychkov::List< T >::compare(const List& rhs, C comp) const
{
  for (const_iterator i = begin(), j = rhs.begin(); (i != end()) && (j != rhs.end()); ++i, ++j)
  {
    if (!comp(*i, *j))
    {
      return false;
    }
  }
  return true;
}

#endif

