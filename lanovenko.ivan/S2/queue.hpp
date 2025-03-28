#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <list>
#include <utility>

namespace lanovenko
{
  template < typename T >
  class Queue
  {
  public:
    ~Queue() = default;
    Queue() = default;
    Queue(const Queue&) = default;
    Queue(Queue&&) = default;
    Queue& operator=(const Queue&) = default;
    Queue& operator=(Queue&&) = default;

    template < typename U >
    void push(U&& data) noexcept;
    void pop() noexcept;
    T& front() noexcept;
    const T& front() const noexcept;

    bool empty() const noexcept;
  private:
    std::list< T > data_;
  };
}

template < typename T >
template < typename U >
void lanovenko::Queue< T >::push(U&& data) noexcept
{
  data_.push_back(std::forward< U >(data));
}

template < typename T >
void lanovenko::Queue< T >::pop() noexcept
{
  data_.pop_front();
}

template < typename T >
T& lanovenko::Queue< T >::front() noexcept
{
  return data_.front();
}

template < typename T >
const T& lanovenko::Queue< T >::front() const noexcept
{
  return data_.front_();
}

template < typename T >
bool lanovenko::Queue< T >::empty() const noexcept
{
  return data_.empty();
}

#endif
