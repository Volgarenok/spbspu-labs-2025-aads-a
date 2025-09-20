#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace zakirov
{
  template < class T >
  class Stack
  {
  public:
    Stack() noexcept;
    Stack(const Stack & other);
    Stack(Stack && other) noexcept;
    ~Stack();
    Stack< T > & operator=(const Stack< T > & other);
    Stack< T > & operator=(Stack< T > && other) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    T & top() noexcept;
    const T & top() const noexcept;
    void push(const T & value);
    void push(T && value);
    void pop() noexcept;
    void swap(Stack< T > & other) noexcept;
  private:
    template < class U >
    void uni_push(U && value);
    void add_capacity();
    T * data_;
    size_t top_;
    size_t capacity_;
  };

  template < class T >
  Stack< T >::Stack() noexcept:
    data_(nullptr),
    top_(0),
    capacity_(0)
  {}

  template < class T >
  Stack< T >::Stack(const Stack & other):
    data_(new T[other.capacity_]),
    top_(other.top_),
    capacity_(other.capacity_)
  {
    for (size_t i = 0; i < top_; ++i)
    {
      data_[i] = other.data_[i];
    }
  }

  template < class T >
  Stack< T >::Stack(Stack && other) noexcept:
    data_(std::exchange(other.data_, nullptr)),
    top_(std::exchange(other.top_, 0)),
    capacity_(std::exchange(other.capacity_, 0))
  {}

  template < class T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }

  template < class T >
  Stack< T > & Stack< T >::operator=(const Stack< T > & other)
  {
    if (this != std::addressof(other))
    {
      Stack< T > stack_temp(other);
      swap(stack_temp);
    }

    return *this;
  }

  template < class T >
  Stack< T > & Stack< T >::operator=(Stack< T > && other) noexcept
  {
    if (this != std::addressof(other))
    {
      Stack< T > stack_temp(std::move(other));
      swap(stack_temp);
    }

    return *this;
  }

  template < class T >
  bool Stack< T >::empty() const noexcept
  {
    return top_ == 0;
  }

  template < class T >
  size_t Stack< T >::size() const noexcept
  {
    return top_;
  }

  template < class T >
  T & Stack< T >::top() noexcept
  {
    return data_[top_ - 1];
  }

  template < class T >
  const T & Stack< T >::top() const noexcept
  {
    return data_[top_ - 1];
  }

  template < class T >
  template < class U >
  void Stack< T >::uni_push(U && value)
  {
    if (top_ >= capacity_)
    {
      add_capacity();
    }

    data_[top_] = std::forward< U >(value);
    ++top_;
  }

  template < class T >
  void Stack< T >::push(const T & value)
  {
    uni_push(value);
  }

  template < class T >
  void Stack< T >::push(T && value)
  {
    uni_push(std::move(value));
  }

  template < class T >
  void Stack< T >::pop() noexcept
  {
    --top_;
  }

  template < class T >
  void Stack< T >::swap(Stack< T > & other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(top_, other.top_);
    std::swap(capacity_, other.capacity_);
  }

  template < class T >
  void Stack< T >::add_capacity()
  {
    size_t new_capacity = capacity_ * 2 + 1;
    T * new_data = new T[new_capacity];
    try
    {
      for (size_t i = 0; i < top_; ++i)
      {
        new_data[i] = std::move(data_[i]);
      }
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }
}

#endif
