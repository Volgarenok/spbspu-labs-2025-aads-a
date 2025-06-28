#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>

namespace dribas
{
  template< class T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack< T >&);
    Stack(Stack< T >&&) noexcept;
    ~Stack();

    Stack< T >& operator=(const Stack< T >&);
    Stack< T >& operator=(Stack< T >&&) noexcept;

    T& top();
    const T& top() const;
    void push(const T&);
    void pop();
    void swap(Stack< T >&) noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;

  private:
    size_t capacity_;
    size_t size_;
    T* stack_;
    void resize(size_t);
  };

  template< class T >
  Stack< T >::Stack():
    capacity_(0),
    size_(0),
    stack_(nullptr)
  {}

  template< class T >
  Stack< T >::Stack(const Stack< T >& other):
    capacity_(other.size_),
    size_(other.size_),
    stack_(size_ ? new T[capacity_] : nullptr)
  {
    try {
      for (size_t i = 0; i < size_; ++i) {
        stack_[i] = other.stack_[i];
      }
    } catch (const std::exception&) {
      delete[] stack_;
      throw;
    }
  }

  template< class T >
  Stack< T >::Stack(Stack< T >&& other) noexcept:
    capacity_(other.capacity_),
    size_(other.size_),
    stack_(other.stack_)
  {
    other.capacity_ = 0;
    other.size_ = 0;
    other.stack_ = nullptr;
  }

  template< class T >
  Stack< T >::~Stack()
  {
    delete[] stack_;
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& other)
  {
    if (this != std::addressof(other)) {
      Stack< T > tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& other) noexcept
  {
    if (this != std::addressof(other)) {
      Stack< T > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< class T >
  void Stack< T >::resize(size_t new_capacity)
  {
    T* new_stack = new T[new_capacity];
    try {
      for (size_t i = 0; i < size_; ++i) {
        new_stack[i] = std::move(stack_[i]);
      }
      delete[] stack_;
      stack_ = new_stack;
      capacity_ = new_capacity;
    } catch (const std::exception&) {
      delete[] new_stack;
      throw;
    }
  }

  template< class T >
  void Stack< T >::push(const T& value)
  {
    if (size_ == capacity_) {
      resize(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    stack_[size_] = value;
    ++size_;
  }

  template< class T >
  void Stack< T >::pop()
  {
    --size_;
    if (capacity_ > 4 && size_ < capacity_ / 4) {
      resize(capacity_ / 2);
    }
  }

  template< class T >
  T& Stack< T >::top()
  {
    return stack_[size_ - 1];
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    return stack_[size_ - 1];
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void Stack< T >::swap(Stack< T >& other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(stack_, other.stack_);
  }
}

#endif
