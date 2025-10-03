#ifndef STACK_HPP
#define STACK_HPP

#include "queue_stack_base.hpp"

namespace maslevtsov {
  template< class T >
  class Stack
  {
  public:
    T& top() noexcept;
    const T& top() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;
    void swap(Stack& other) noexcept;

  private:
    QueueStackBase< T, false > stack_base_;
  };
}

template< class T >
T& maslevtsov::Stack< T >::top() noexcept
{
  return stack_base_[stack_base_.size() - 1];
}

template< class T >
const T& maslevtsov::Stack< T >::top() const noexcept
{
  return stack_base_[stack_base_.size() - 1];
}

template< class T >
bool maslevtsov::Stack< T >::empty() const noexcept
{
  return stack_base_.empty();
}

template< class T >
std::size_t maslevtsov::Stack< T >::size() const noexcept
{
  return stack_base_.size();
}

template< class T >
void maslevtsov::Stack< T >::push(const T& value)
{
  stack_base_.push(value);
}

template< class T >
void maslevtsov::Stack< T >::push(T&& value)
{
  stack_base_.push(std::move(value));
}

template< class T >
void maslevtsov::Stack< T >::pop() noexcept
{
  stack_base_.pop();
}

template< class T >
void maslevtsov::Stack< T >::swap(Stack& other) noexcept
{
  stack_base_.swap(other.stack_base_);
}

#endif
