#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace gavrilova {
  template < class T >
  class Stack {
  public:
    Stack();
    Stack(const Stack< T >& other);
    Stack(Stack< T >&& other);
    ~Stack();

    Stack< T >& operator=(const Stack& other);
    Stack< T >& operator=(Stack&& other);

    void push(const T& value);
    void push(T&& value);

    T& top();
    void pop();
    bool empty() const;
    size_t size() const;
    void swap(Stack& other ) noexcept;

  private:
    size_t size_;
    size_t capacity_;
    T* stack_;

    void resize();
  };

  template < class T >
  Stack< T >::Stack():
    size_(0),
    capacity_(50),
    stack_(new T[capacity_])
  {}

  template < class T >
  Stack< T >::Stack(const Stack< T >& other):
    size_(other.size_),
    capacity_(other.capacity_),
    stack_(new T[capacity_])
  {
    for  (size_t i = 0; i < size_; ++i) {
      stack_[i] = other.stack_[i];
    }
  }

  template < class T >
  Stack< T >::Stack(Stack<T>&& other):
    size_(other.size_),
    capacity_(other.capacity_),
    stack_(other.stack_)
  {
    other.size_ = 0;
    other.capacity_ = 0;
    other.stack_ = nullptr;
  }

  template < class T >
  Stack< T >::~Stack()
  {
    delete[] stack_;
  }

  template < class T >
  Stack< T >& Stack< T >::operator=(const Stack& other)
  {
    if (this != std::addressof(other)) {
      Stack cpy(other);
      swap(cpy);
    }
    return *this;
  }

  template < class T >
  Stack< T >& Stack< T >::operator=(Stack&& other)
  {
    if (this != std::addressof(other)) {
      swap(other);
      delete[] other.stack_;
    }
    return *this;
  }

  template < class T >
  void Stack< T >::push(const T& value)
  {
    if (size_ == capacity_) {
      resize();
    }
    stack_[size_++] = value;
  }

  template < class T >
  void Stack< T >::push(T&& value)
  {
    if (size_ == capacity_) {
      resize();
    }
    stack_[size_++] = std::move(value);
  }

  template < class T >
  T& Stack< T >::top()
  {
    return stack_[size_ - 1];
  }

  template < class T >
  void Stack< T >::pop()
  {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    --size_;
  }

  template < class T >
  bool Stack< T >::empty() const
  {
    return !size_;
  }

  template < class T >
  size_t Stack< T >::size() const
  {
    return size_;
  }

  template < class T >
  void Stack< T >::swap(Stack& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(stack_, other.stack_);
  }

  template < class T >
  void Stack< T >::resize()
  {
    T* new_stack = new T[capacity_+50];
    for (size_t i = 0; i < size_; ++i) {
      new_stack[i] = stack_[i];
    }
    delete[] stack_;
    stack_ = new_stack;
    capacity_ += 50;
  }

}

#endif
