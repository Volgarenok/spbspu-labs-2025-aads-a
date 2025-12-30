#ifndef STACK_HPP
#define STACK_HPP
#include "queue.hpp"
namespace nikonov
{
  template< typename T >
  class Stack
  {
    Array< T > arr_;
  public:
    Stack();
    Stack(const Stack & copy);
    Stack(Stack && copy);
    ~Stack() = default;

    bool empty() const noexcept;
    size_t size() const noexcept;
    T & top() noexcept;
    const T & top() const noexcept;
    void push(const T & val);
    void push(T && val);
    void pop() noexcept;
    void swap(Stack & x) noexcept;
  };
}

template< typename T >
nikonov::Stack< T >::Stack():
  arr_()
{}
template< typename T >
nikonov::Stack< T >::Stack(const Stack & copy):
  arr_(copy.arr_)
{}
template< typename T >
nikonov::Stack< T >::Stack(Stack && copy):
  arr_(std::move(copy.arr_))
{}

template< typename T >
bool nikonov::Stack< T >::empty() const noexcept
{
  return arr_.empty();
}
template< typename T >
size_t nikonov::Stack< T >::size() const noexcept
{
  return arr_.size();
}
template< typename T >
T & nikonov::Stack< T >::top() noexcept
{
  return arr_.back();
}
template< typename T >
const T & nikonov::Stack< T >::top() const noexcept
{
  return arr_.back();
}
template< typename T >
void nikonov::Stack< T >::push(const T & val)
{
  arr_.push_back(val);
}
template< typename T >
void nikonov::Stack< T >::push(T && val)
{
  arr_.push_back(val);
}
template< typename T >
void nikonov::Stack< T >::pop() noexcept
{
  assert(arr_.size() > 0);
  arr_.pop_back();
}
template< typename T >
void nikonov::Stack< T >::swap(Stack & x) noexcept
{
  arr_.swap(x.arr_);
}
#endif
