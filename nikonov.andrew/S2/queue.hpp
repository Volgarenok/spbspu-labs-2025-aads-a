#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <cassert>
#include "dynamicArray.hpp"
namespace nikonov
{
  template< typename T >
  class Queue
  {
    Array< T > arr_;
  public:
    Queue();
    Queue(const Queue & copy);
    Queue(Queue && copy);
    ~Queue() = default;

    bool empty() const noexcept;
    size_t size() const noexcept;
    T & front() noexcept;
    const T & front() const noexcept;
    T & back() noexcept;
    const T & back() const noexcept;
    void push(const T & val);
    void push(T && val);
    void pop();
    void swap(Queue & x) noexcept;
  };
}

template< typename T >
nikonov::Queue< T >::Queue():
  arr_()
{}
template< typename T >
nikonov::Queue< T >::Queue(const Queue & copy):
  arr_(copy.arr_)
{}
template< typename T >
nikonov::Queue< T >::Queue(Queue && copy):
  arr_(std::move(copy.arr_))
{}

template< typename T >
bool nikonov::Queue< T >::empty() const noexcept
{
  return arr_.empty();
}
template< typename T >
size_t nikonov::Queue< T >::size() const noexcept
{
  return arr_.size();
}
template< typename T >
T & nikonov::Queue< T >::front() noexcept
{
  return arr_.front();
}
template< typename T >
const T & nikonov::Queue< T >::front() const noexcept
{
  return arr_.front();
}
template< typename T >
T & nikonov::Queue< T >::back() noexcept
{
  return arr_.back();
}
template< typename T >
const T & nikonov::Queue< T >::back() const noexcept
{
  return arr_.back();
}
template< typename T >
void nikonov::Queue< T >::push(const T & val)
{
  arr_.push_back(val);
}
template< typename T >
void nikonov::Queue< T >::push(T && val)
{
  arr_.push_back(val);
}
template< typename T >
void nikonov::Queue< T >::pop()
{
  arr_.pop_front();
}
template< typename T >
void nikonov::Queue< T >::swap(Queue & x) noexcept
{
  arr_.swap(x.arr_);
}
#endif
