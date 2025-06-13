#ifndef STACK_HPP
#define STACK_HPP

#include <cassert>
#include <cstddef>
#include <utility>

namespace lanovenko
{
  template< typename T >
  class Stack
  {
  public:
    ~Stack();
    Stack();
    Stack(const Stack& rhs);
    Stack(Stack&& rhs) noexcept;
    Stack& operator=(const Stack& rhs);
    Stack& operator=(Stack&& rhs) noexcept;
    bool empty() const noexcept;
    void swap(Stack& rhs) noexcept;
    void pop() noexcept;
    void push(const T& value);
    size_t size() const noexcept;
    T& top();
  private:
    const size_t max_size_;
    size_t size_;
    T* data_;
  };
}

template< typename T >
lanovenko::Stack< T >::~Stack()
{
  delete[] data_;
  size_ = 0;
  data_ = nullptr;
}

template< typename T >
lanovenko::Stack< T >::Stack():
  max_size_(1'000),
  size_(0),
  data_(new T[max_size_])
{}

template< typename T >
lanovenko::Stack< T >::Stack(const Stack& rhs):
  max_size_(rhs.max_size_),
  size_(rhs.size_),
  data_(new T[max_size_]{})
{
  Stack< T > temporary{};
  for (size_t i = 0; i < rhs.size_; i++)
  {
    temporary.push(rhs.data_[i]);
  }
  swap(temporary);
}

template< typename T >
lanovenko::Stack< T >::Stack(Stack&& rhs) noexcept:
  max_size_(rhs.max_size_),
  size_(rhs.size_),
  data_(rhs.data_)
{
  rhs.size_ = 0;
  rhs.data_ = nullptr;
}

template< typename T >
lanovenko::Stack< T >& lanovenko::Stack< T >::operator=(const Stack& rhs)
{
  if (this != &rhs)
  {
    Stack< T > temporary(rhs);
    swap(temporary);
  }
  return *this;
}

template<typename T>
lanovenko::Stack< T >& lanovenko::Stack< T >::operator=(Stack&& rhs) noexcept
{
  if (this != &rhs)
  {
    Stack< T > temporary(std::move(rhs));
    swap(temporary);
  }
  return *this;
}

template<typename T>
inline bool lanovenko::Stack< T >::empty() const noexcept
{
  return size_ == 0;
}

template < typename T >
inline void lanovenko::Stack< T >::swap(Stack& rhs) noexcept
{
  using std::swap;
  swap(size_, rhs.size_);
  swap(data_, rhs.data_);
}

template<typename T>
void lanovenko::Stack< T >::pop() noexcept
{
  assert(!this->empty());
  size_--;
}

template< typename T >
void lanovenko::Stack< T >::push(const T& rhs)
{
  assert(size_ < max_size_);
  data_[size_++] = rhs;
}

template<typename T>
inline size_t lanovenko::Stack< T >::size() const noexcept
{
  return size_;
}

template< typename T >
T& lanovenko::Stack< T >::top()
{
  assert(!this->empty());
  return data_[size_ - 1];
}

#endif
