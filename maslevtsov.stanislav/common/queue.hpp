#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "queue_stack_base.hpp"

namespace maslevtsov {
  template< class T >
  class Queue
  {
  public:
    T& front() noexcept;
    const T& front() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;
    void swap(Queue& other) noexcept;

  private:
    QueueStackBase< T, true > queue_base_;
  };
}

template< class T >
T& maslevtsov::Queue< T >::front() noexcept
{
  return queue_base_[0];
}

template< class T >
const T& maslevtsov::Queue< T >::front() const noexcept
{
  return queue_base_[0];
}

template< class T >
bool maslevtsov::Queue< T >::empty() const noexcept
{
  return queue_base_.empty();
}

template< class T >
std::size_t maslevtsov::Queue< T >::size() const noexcept
{
  return queue_base_.size();
}

template< class T >
void maslevtsov::Queue< T >::push(const T& value)
{
  queue_base_.push(value);
}

template< class T >
void maslevtsov::Queue< T >::push(T&& value)
{
  queue_base_.push(std::move(value));
}

template< class T >
void maslevtsov::Queue< T >::pop() noexcept
{
  queue_base_.pop();
}

template< class T >
void maslevtsov::Queue< T >::swap(Queue& other) noexcept
{
  queue_base_.swap(other.queue_base_);
}

#endif
