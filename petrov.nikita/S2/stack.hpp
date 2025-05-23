#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>

namespace petrov
{
  template< typename T >
  struct Stack
  {
  public:
    using this_t = Stack< T >;
    Stack():
      massive_(nullptr),
      capacity_(0),
      size_of_stack_(0)
    {}
    Stack(const this_t & rhs);
    Stack(this_t && rhs);
    ~Stack();
    this_t & operator=(const this_t & rhs);
    this_t & operator=(this_t && rhs);
    void push(const T & element);
    void pop();
    T & top();
    const T & top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(this_t & rhs) noexcept;
  private:
    T * massive_;
    size_t capacity_;
    size_t size_of_stack_;
  };

  template< typename T >
  Stack< T >::Stack(const this_t & rhs):
    massive_(nullptr),
    capacity_(rhs.capacity_),
    size_of_stack_(rhs.size_of_stack_)
  {
    massive_ = new T[rhs.size_of_stack_];
    for (size_t i = 0; i < rhs.size_of_stack_; i++)
    {
      massive_[i] = rhs.massive_[i];
    }
  }

  template< typename T >
  Stack< T >::Stack(this_t && rhs):
    massive_(nullptr),
    capacity_(rhs.capacity_),
    size_of_stack_(rhs.size_of_stack_)
  {
    massive_ = new T[rhs.size_of_stack_];
    for (size_t i = 0; i < rhs.size_of_stack_; i++)
    {
      massive_[i] = rhs.massive_[i];
    }
  }

  template< typename T >
  Stack< T >::~Stack()
  {
    delete[] massive_;
  }

  template< typename T >
  typename Stack< T >::this_t & Stack< T >::operator=(const this_t & rhs)
  {
    Stack< T > cpy(rhs);
    this->swap(cpy);
    return *this;
  }

  template< typename T >
  typename Stack< T >::this_t & Stack< T >::operator=(this_t && rhs)
  {
    this->swap(rhs);
    return *this;
  }

  template< typename T >
  void Stack< T >::push(const T & element)
  {
    if (this->empty())
    {
      delete[] massive_;
      capacity_ = 5;
      massive_ = new T[capacity_];
      massive_[size_of_stack_++] = element;
    }
    else if (size_of_stack_ == capacity_)
    {
      T * temp = new T[capacity_ *= 2];
      size_t i = 0;
      while (i < size_of_stack_)
      {
        temp[i] = massive_[i];
        ++i;
      }
      delete[] massive_;
      massive_ = temp;
      massive_[i] = element;
      size_of_stack_++;
    }
    else
    {
      massive_[size_of_stack_++] = element;
    }
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (this->empty())
    {
      return;
    }
    size_of_stack_--;
  }

  template< typename T >
  T & Stack< T >::top()
  {
    if (this->empty())
    {
      throw std::logic_error("ERROR: Stack is empty");
    }
    return massive_[size_of_stack_ - 1];
  }

  template< typename T >
  const T & Stack< T >::top() const
  {
    if (this->empty())
    {
      throw std::logic_error("ERROR: Stack is empty");
    }
    return massive_[size_of_stack_ - 1];
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return !size_of_stack_;
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return size_of_stack_;
  }

  template< typename T >
  void Stack< T >::swap(this_t & rhs) noexcept
  {
    std::swap(massive_, rhs.massive_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_of_stack_, rhs.size_of_stack_);
  }
}
#endif
