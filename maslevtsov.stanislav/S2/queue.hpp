#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <utility>
#include <stdexcept>

namespace maslevtsov {
  template< class T >
  class Queue
  {
  public:
    Queue() noexcept;
    Queue(const Queue& rhs);
    Queue(Queue&& rhs) noexcept;
    ~Queue();

    Queue& operator=(const Queue& rhs);
    Queue& operator=(Queue&& rhs) noexcept;

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;
    void swap(Queue& other) noexcept;

  private:
    T* data_;
    std::size_t size_, first_, capacity_;

    template< class U >
    void push_impl(U&& value);
    void expand_data(std::size_t new_capacity);
  };
}

template< class T >
maslevtsov::Queue< T >::Queue() noexcept:
  data_(nullptr),
  size_(0),
  first_(0),
  capacity_(0)
{}

template< class T >
maslevtsov::Queue< T >::Queue(const Queue& rhs):
  data_(new T[rhs.capacity_]),
  size_(rhs.size_),
  first_(0),
  capacity_(rhs.capacity_)
{
  try {
    for (std::size_t i = 0, j = rhs.first_; i != capacity_; ++i, j = (j + 1) % capacity_) {
      data_[i] = rhs.data_[j];
    }
  } catch (const std::exception&) {
    delete[] data_;
    throw;
  }
}

template< class T >
maslevtsov::Queue< T >::Queue(Queue&& rhs) noexcept:
  data_(std::exchange(rhs.data_, nullptr)),
  size_(std::exchange(rhs.size_, 0)),
  first_(std::exchange(rhs.first_, 0)),
  capacity_(std::exchange(rhs.capacity_, 0))
{}

template< class T >
maslevtsov::Queue< T >::~Queue()
{
  delete[] data_;
}

template< class T >
maslevtsov::Queue< T >& maslevtsov::Queue< T >::operator=(const Queue& rhs)
{
  Queue< T > copied(rhs);
  swap(copied);
  return *this;
}

template< class T >
maslevtsov::Queue< T >& maslevtsov::Queue< T >::operator=(Queue&& rhs) noexcept
{
  Queue< T > moved(std::move(rhs));
  swap(rhs);
  return *this;
}

template< class T >
T& maslevtsov::Queue< T >::front() noexcept
{
  return data_[first_];
}

template< class T >
const T& maslevtsov::Queue< T >::front() const noexcept
{
  return data_[first_];
}

template< class T >
T& maslevtsov::Queue< T >::back() noexcept
{
  return data_[size_ - 1];
}

template< class T >
const T& maslevtsov::Queue< T >::back() const noexcept
{
  return data_[size_ - 1];
}

template< class T >
bool maslevtsov::Queue< T >::empty() const noexcept
{
  return size_ == 0;
}

template< class T >
std::size_t maslevtsov::Queue< T >::size() const noexcept
{
  return size_;
}

template< class T >
void maslevtsov::Queue< T >::push(const T& value)
{
  push_impl(value);
}

template< class T >
void maslevtsov::Queue< T >::push(T&& value)
{
  push_impl(std::move(value));
}

template< class T >
void maslevtsov::Queue< T >::pop() noexcept
{
  first_ = (first_ + 1) % capacity_;
  --size_;
}

template< class T >
void maslevtsov::Queue< T >::swap(Queue& other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(first_, other.first_);
  std::swap(capacity_, other.capacity_);
}

template< class T >
template< class U >
void maslevtsov::Queue< T >::push_impl(U&& value)
{
  if (size_ == capacity_) {
    expand_data(size_ * 2 + 1);
  }
  data_[size_++] = std::forward< U >(value);
}

template< class T >
void maslevtsov::Queue< T >::expand_data(std::size_t new_capacity)
{
  T* new_data = new T[new_capacity];
  try {
    for (std::size_t i = 0, j = first_; i != capacity_; ++i, j = (j + 1) % capacity_) {
      new_data[i] = data_[j];
    }
  } catch (const std::exception&) {
    delete[] new_data;
    throw;
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_capacity;
  first_ = 0;
}

#endif
