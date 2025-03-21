#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <list>

namespace maslevtsov {
  template< class T >
  class Queue
  {
  public:
    Queue() noexcept = default;
    Queue(const Queue& rhs) = default;
    Queue(Queue&& rhs) noexcept = default;
    ~Queue() = default;

    Queue& operator=(const Queue& rhs) = default;
    Queue& operator=(Queue&& rhs) noexcept = default;

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;

  private:
    std::list< T > data_;
  };
}

template< class T >
T& maslevtsov::Queue< T >::front() noexcept
{
  return data_.front();
}

template< class T >
const T& maslevtsov::Queue< T >::front() const noexcept
{
  return data_.front();
}

template< class T >
T& maslevtsov::Queue< T >::back() noexcept
{
  return data_.back();
}

template< class T >
const T& maslevtsov::Queue< T >::back() const noexcept
{
  return data_.back();
}

template< class T >
bool maslevtsov::Queue< T >::empty() const noexcept
{
  return data_.empty();
}

template< class T >
std::size_t maslevtsov::Queue< T >::size() const noexcept
{
  return data_.size();
}

template< class T >
void maslevtsov::Queue< T >::push(const T& value)
{
  data_.push_back(value);
}

template< class T >
void maslevtsov::Queue< T >::push(T&& value)
{
  data_.push_back(std::move(value));
}

template< class T >
void maslevtsov::Queue< T >::pop() noexcept
{
  data_.pop_front();
}

#endif
