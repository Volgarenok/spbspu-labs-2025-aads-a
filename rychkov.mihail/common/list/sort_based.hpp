#ifndef SORT_BASED_HPP
#define SORT_BASED_HPP

#include "declaration.hpp"

#include <functional>
#include <utility>

template< class T >
typename rychkov::List< T >::size_type rychkov::List< T >::unique()
{
  return unique(std::equal_to<>());
}
template< class T >
template< class P >
typename rychkov::List< T >::size_type rychkov::List< T >::unique(P predicate)
{
  size_type result = 0;
  const_iterator i = ++begin(), prev = begin();
  while (i != end())
  {
    if (predicate(*i, *prev))
    {
      const_iterator temp = i++;
      erase(temp);
      result++;
    }
    else
    {
      prev = i++;
    }
  }
  return result;
}
template< class T >
void rychkov::List< T >::sort()
{
  sort(std::less<>());
}
template< class T >
template< class C >
void rychkov::List< T >::sort(C compare)
{
  if (size_ <= 1)
  {
    return;
  }
  const_iterator from = begin();
  const_iterator to = merge_sort(compare, from, size_);
  head_ = from.node_;
  tail_ = to.node_;
  head_->prev = nullptr;
  tail_->next = nullptr;
}
template< class T >
template< class C >
typename rychkov::List< T >::const_iterator rychkov::List< T >::merge_sort(C& compare, const_iterator& from, size_t count)
{
  if (count == 1)
  {
    return from;
  }
  const_iterator mid = merge_sort(compare, from, (count + 1) / 2);
  const_iterator postMid = ++const_iterator{mid};
  const_iterator to = merge_sort(compare, postMid, count / 2);
  return unsafeMerge(compare, from, mid, postMid, to);
}
template< class T >
template< class C >
typename rychkov::List< T >::const_iterator rychkov::List< T >::unsafeMerge(C& compare,
    const_iterator& leftFrom, const_iterator leftTo, const_iterator rightFrom, const_iterator rightTo)
{
  const_iterator insTail;
  const_iterator i = leftFrom, j = rightFrom;
  const_iterator postTail = ++const_iterator(rightTo);
  if (compare(*rightFrom, *leftFrom))
  {
    insTail = j++;
  }
  else
  {
    insTail = i++;
  }
  leftFrom = insTail;
  const_iterator endL = ++const_iterator(leftTo), endR = ++const_iterator(rightTo);
  while ((i != endL) && (j != endR))
  {
    if (compare(*j, *i))
    {
      insTail.node_->next = j.node_;
      j.node_->prev = insTail.node_;
      insTail = j++;
    }
    else
    {
      insTail.node_->next = i.node_;
      i.node_->prev = insTail.node_;
      insTail = i++;
    }
  }
  if (i != endL)
  {
    insTail.node_->next = i.node_;
    i.node_->prev = insTail.node_;
    leftTo.node_->next = postTail.node_;
    return leftTo;
  }
  else
  {
    insTail.node_->next = j.node_;
    j.node_->prev = insTail.node_;
    return rightTo;
  }
}
template< class T >
void rychkov::List< T >::merge(List& rhs)
{
  merge(rhs, std::less<>());
}
template< class T >
void rychkov::List< T >::merge(List&& rhs)
{
  merge(rhs);
}
template< class T >
template< class C >
void rychkov::List< T >::merge(List& rhs, C compare)
{
  if (rhs.begin() == rhs.end())
  {
    return;
  }
  const_iterator from = begin();
  const_iterator to = unsafeMerge(compare, from, {tail_, tail_}, rhs.begin(), {rhs.tail_, rhs.tail_});
  size_ += rhs.size_;
  head_ = from.node_;
  tail_ = to.node_;
  head_->prev = nullptr;
  tail_->next = nullptr;
  rhs.head_ = nullptr;
  rhs.tail_ = nullptr;
  rhs.size_ = 0;
}
template< class T >
template< class C >
void rychkov::List< T >::merge(List&& rhs, C compare)
{
  merge(rhs, std::move(compare));
}

#endif
