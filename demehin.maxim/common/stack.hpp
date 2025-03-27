#ifndef STACK_HPP
#define STACK_HPP
#include <utility>
#include <cstddef>
#include <stdexcept>
#include "data_utils.hpp"

namespace demehin
{
  template< typename T >
  class Stack
  {
  public:

    Stack();
    Stack(const Stack&);
    Stack(Stack&&);

    Stack< T >& operator=(const Stack< T >&);
    ~Stack();

    void push(T rhs);
    const T& top() const;
    T& top();
    void pop();

    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void resize();
  };

  template< typename T >
  Stack< T >::Stack():
    data_(nullptr),
    size_(0),
    capacity_(100)
  {
    data_ = new T[capacity_];
  }

  template< typename T >
  Stack< T >::Stack(const Stack& rhs):
    data_(nullptr),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {
    data_ = details::copyData(rhs.data_, rhs.size_);
  }

  template< typename T >
  Stack< T >::Stack(Stack&& rhs):
    data_(std::exchange(rhs.data_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0))
  {}

  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& rhs)
  {
    if (this != &rhs)
    {
      Stack< T > temp(rhs);
      std::swap(data_, temp.data_);
      std::swap(size_, temp.size_);
      std::swap(capacity_, temp.capacity_);
    }
    return *this;
  }

  template< typename T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }

  template< typename T >
  void Stack< T >::push(T rhs)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    data_[size_++] = rhs;
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  T& Stack< T >::top()
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty2");
    }
    size_--;
  }

  template< typename T >
  void Stack< T >::resize()
  {
    capacity_ *= 2;
    T* newData = details::copyData(data_, capacity_);
    delete[] data_;
    data_ = newData;
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return size_;
  }
}

#endif
