#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cassert>
#include <cstddef>
#include <utility>

namespace lanovenko
{
  template< typename T >
  class Queue
  {
  public:
    ~Queue();
    Queue();
    Queue(const Queue& rhs);
    Queue(Queue&& rhs) noexcept;
    Queue& operator=(const Queue& rhs);
    Queue& operator=(Queue&& rhs) noexcept;
    void swap(Queue& rhs) noexcept;
    void push(const T& data);
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front() noexcept;
    T& back() noexcept;
  private:
    const size_t max_size_;
    size_t size_;
    size_t first_;
    T* data_;
  };
}

template< typename T >
lanovenko::Queue< T >::~Queue()
{
  size_ = first_ = 0;
  delete[] data_;
  data_ = nullptr;
}

template< typename T >
lanovenko::Queue< T >::Queue():
  max_size_(1'000),
  size_(0),
  first_(0),
  data_(new T[max_size_])
{}

template< typename T >
lanovenko::Queue< T >::Queue(const Queue& rhs):
  max_size_(rhs.max_size_),
  size_(rhs.size_),
  first_(rhs.first_),
  data_(new T[max_size_])
{
  Queue< T > temporary{};
  for (size_t i = 0; i < rhs.size_; i++)
  {
    temporary.push(rhs.data_[i]);
  }
  swap(temporary);
}

template< typename T >
lanovenko::Queue< T >::Queue(Queue&& rhs) noexcept:
  max_size_(rhs.max_size_),
  size_(rhs.size_),
  first_(rhs.first_),
  data_(rhs.data_)
{
  rhs.size_ = rhs.first_ = 0;
  rhs.data_ = nullptr;
}

template< typename T >
lanovenko::Queue< T >& lanovenko::Queue< T >::operator=(const Queue& rhs)
{
  if (this != &rhs)
  {
    Queue< T > temporary(rhs);
    swap(temporary);
  }
  return *this;
}

template< typename T >
lanovenko::Queue< T >& lanovenko::Queue< T >::operator=(Queue&& rhs) noexcept
{
  if (this != &rhs)
  {
    Queue< T > temporary(std::move(rhs));
    swap(temporary);
  }
  return *this;
}

template< typename T >
inline void lanovenko::Queue< T >::swap(Queue& rhs) noexcept
{
  using std::swap;
  swap(size_, rhs.size_);
  swap(first_, rhs.first_);
  swap(data_, rhs.data_);
}

template< typename T >
void lanovenko::Queue< T >::push(const T& data)
{
  assert(size_ < max_size_);
  data_[size_++] = data;
}

template< typename T >
void lanovenko::Queue< T >::pop() noexcept
{
  assert(!this->empty());
  size_--;
  if (size_ != 0)
  {
    first_++;
  }
}

template< typename T >
inline bool lanovenko::Queue< T >::empty() const noexcept
{
  return size_ == 0;
}

template< typename T >
inline size_t lanovenko::Queue< T >::size() const noexcept
{
  return size_;
}

template< typename T >
T& lanovenko::Queue< T >::front() noexcept
{
  assert(!this->empty());
  return data_[first_];
}

template< typename T >
T& lanovenko::Queue< T >::back() noexcept
{
  assert(!this->empty());
  return data_[size_ - 1];
}

#endif
