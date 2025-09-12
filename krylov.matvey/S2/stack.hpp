#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <exception>

namespace krylov
{
  template< typename T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack< T >& other);
    Stack(Stack< T >&& other);
    ~Stack();
    Stack< T >& operator=(const Stack< T >& other);
    Stack< T >& operator=(Stack< T >&& other);
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& top() noexcept;
    const T& top() const noexcept;
    void push(const T& value);
    void pop() noexcept;
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
    capacity_(0)
  {}

  template< typename T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }

  template< typename T >
  Stack< T >::Stack(const Stack< T >& other):
    data_(nullptr),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    T* data = new T[other.capacity_];
    try
    {
      for (size_t i = 0; i < other.size(); ++i)
      {
        data[i] = other.data_[i];
      }
    }
    catch (const std::exception& e)
    {
      delete[] data;
    }
  }

  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& other)
  {
    if (this == &other)
    {
      return *this;
    }
    Stack< T > newStack(other);
    data_ = newStack.data_;
    size_ = newStack.size_;
    capacity_ = newStack.capacity_;
    return *this;
  }

  template< typename T >
  Stack< T >::Stack(Stack< T >&& other):
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
  }

  template< typename T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& other)
  {
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
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

  template< typename T >
  T& Stack< T >::top() noexcept
  {
    return data_[size_-1];
  }

  template< typename T >
  const T& Stack< T >::top() const noexcept
  {
    return data_[size_-1];
  }

  template< typename T >
  void Stack< T >::push(const T& value)
  {
    if (size_ != capacity_)
    {
      data_[size_] = value;
    }
    else
    {
      resize();
      data_[size_] = value;
    }
    ++size_;
  }

  template< typename T >
  void Stack< T >::pop() noexcept
  {
    if (!empty())
    {
      --size_;
    }
  }

  template< typename T >
  void Stack< T >::resize()
  {
    constexpr size_t k = 161;
    T* array = new T[capacity_ + k];
    for (size_t i = 0; i < size_; ++i)
    {
      array[i] = data_[i];
    }
    delete[] data_;
    data_ = array;
    capacity_ += k;
  }
}

#endif
