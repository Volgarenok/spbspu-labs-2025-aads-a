#ifndef STACK_HPP
#define STACK_HPP

#include <utility>
#include <stdexcept>

namespace maslevtsov {
  template< class T >
  class Stack
  {
  public:
    Stack() noexcept;
    Stack(const Stack& rhs);
    Stack(Stack&& rhs) noexcept;
    ~Stack();

    Stack& operator=(const Stack& rhs);
    Stack& operator=(Stack&& rhs) noexcept;

    T& top() noexcept;
    const T& top() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;
    void swap(Stack& other) noexcept;

  private:
    T* data_;
    std::size_t size_, capacity_;

    template< class U >
    void push_impl(U&& value);
    void expand_data(std::size_t new_capacity);
  };
}

template< class T >
maslevtsov::Stack< T >::Stack() noexcept:
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
maslevtsov::Stack< T >::Stack(const Stack& rhs):
  data_(new T[rhs.capacity_]),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  try {
    for (std::size_t i = 0; i != capacity_; ++i) {
      data_[i] = rhs.data_[i];
    }
  } catch (const std::exception&) {
    delete[] data_;
    throw;
  }
}

template< class T >
maslevtsov::Stack< T >::Stack(Stack&& rhs) noexcept:
  data_(std::exchange(rhs.data_, nullptr)),
  size_(std::exchange(rhs.size_, 0)),
  capacity_(std::exchange(rhs.capacity_, 0))
{}

template< class T >
maslevtsov::Stack< T >::~Stack()
{
  delete[] data_;
}

template< class T >
maslevtsov::Stack< T >& maslevtsov::Stack< T >::operator=(const Stack& rhs)
{
  Stack< T > copied(rhs);
  swap(copied);
  return *this;
}

template< class T >
maslevtsov::Stack< T >& maslevtsov::Stack< T >::operator=(Stack&& rhs) noexcept
{
  Stack< T > moved(std::move(rhs));
  swap(rhs);
  return *this;
}

template< class T >
T& maslevtsov::Stack< T >::top() noexcept
{
  return data_[size_ - 1];
}

template< class T >
const T& maslevtsov::Stack< T >::top() const noexcept
{
  return data_[size_ - 1];
}

template< class T >
bool maslevtsov::Stack< T >::empty() const noexcept
{
  return size_ == 0;
}

template< class T >
std::size_t maslevtsov::Stack< T >::size() const noexcept
{
  return size_;
}

template< class T >
void maslevtsov::Stack< T >::push(const T& value)
{
  push_impl(value);
}

template< class T >
void maslevtsov::Stack< T >::push(T&& value)
{
  push_impl(std::move(value));
}

template< class T >
void maslevtsov::Stack< T >::pop() noexcept
{
  --size_;
}

template< class T >
void maslevtsov::Stack< T >::swap(Stack& other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template< class T >
template< class U >
void maslevtsov::Stack< T >::push_impl(U&& value)
{
  if (size_ == capacity_) {
    expand_data(size_ * 2 + 1);
  }
  data_[size_++] = std::forward< U >(value);
}

template< class T >
void maslevtsov::Stack< T >::expand_data(std::size_t new_capacity)
{
  T* new_data = new T[new_capacity];
  try {
    for (std::size_t i = 0; i != capacity_; ++i) {
      new_data[i] = data_[i];
    }
  } catch (const std::exception&) {
    delete[] new_data;
    throw;
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_capacity;
}

#endif
