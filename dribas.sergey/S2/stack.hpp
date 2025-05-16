#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <memory>
#include <utility>

namespace dribas
{
  template < class T >
  class Stack {
  public:
    Stack();
    Stack(const Stack< T >&);
    Stack(Stack< T >&&);
    ~Stack();

    Stack& operator=(const Stack&);
    Stack& operator=(Stack&&);

    T& top();
    const T& top() const;
    void pop();
    void push(const T&);
    void swap(Stack&) noexcept;
    size_t size();
    bool empty();

  private:
    size_t size_;
    T* stack_;
  };

  template< class T >
  bool Stack< T >::empty()
  {
    return size_ == 0;
  }

  template< class T >
  size_t Stack< T >::size()
  {
    return size_;
  }

  template< class T >
  Stack< T >::Stack():
    size_(0),
    stack_(nullptr)
  {}

  template< class T >
  Stack< T >::Stack(const Stack< T >& other):
    size_(other.size_),
    stack_(new T[size_])
  {
    for (size_t i = 0; i < size_; i++) {
      stack_[i] = other.stack_[i];
    }
  }

  template< class T >
  Stack< T >::Stack(Stack< T >&& other):
    size_(other.size_),
    stack_(other.stack_)
  {
    other.size_ = 0;
    other.stack_ = nullptr;
  }

  template< class T >
  Stack< T >::~Stack()
  {
    delete[] stack_;
    size_ = 0;
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& other)
  {
    if (this != std::addressof(other)) {
      Stack cpy(other);
      swap(cpy);
    }
    return *this;
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& other)
  {
    if (this != std::addressof(other)) {
      swap(other);
      delete[] other.stack_;
      other.size_ = 0;
    }
    return *this;
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
  void Stack< T >::pop()
  {
    size_--;
  }

  template< class T >
  void Stack< T >::push(const T& value)
  {
    T* new_stack = new T[size_ + 1];

    for (size_t i = 0; i < size_; ++i) {
      new_stack[i] = stack_[i];
    }
    size_++;
    new_stack[size_ - 1] = value;
    delete[] stack_;
    stack_ = new_stack;
  }

  template< class T >
  void Stack< T >::swap(Stack& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(stack_, other.stack_);
  }
}

#endif
