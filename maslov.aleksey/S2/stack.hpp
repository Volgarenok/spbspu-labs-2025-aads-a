#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <stdexcept>

namespace maslov
{
  template< typename T >
  struct Stack
  {
    Stack();
    Stack(const Stack< T > & rhs);
    Stack(Stack< T > && rhs) noexcept;
    ~Stack();
    Stack< T > & operator=(const Stack< T > & rhs);
    Stack< T > & operator=(Stack< T > && rhs) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push(const T & data);
    void pop();
    T & top();
    const T & top() const;
    void swap(Stack< T > & rhs) noexcept;
   private:
    size_t capacity_;
    size_t size_;
    T * data_;
  };

  template< typename T >
  Stack< T >::Stack():
    capacity_(0),
    size_(0),
    data_(new T[capacity_])
  {}

  template< typename T >
  Stack< T >::Stack(const Stack< T > & rhs):
    capacity_(rhs.capacity_),
    size_(rhs.size_),
    data_(new T[capacity_])
  {
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i] = rhs.data_[i];
      }
    }
    catch (const std::exception &)
    {
      delete[] data_;
      throw;
    }
  }

  template< typename T >
  Stack< T >::Stack(Stack< T > && rhs) noexcept:
    capacity_(rhs.capacity_),
    size_(rhs.size_),
    data_(rhs.data_)
  {
    rhs.capacity_ = 0;
    rhs.size_ = 0;
    rhs.data_ = nullptr;
  }

  template< typename T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }

  template< typename T >
  Stack< T > & Stack< T >::operator=(const Stack< T > & rhs)
  {
    if (this != std::addressof(rhs))
    {
      Stack< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Stack< T > & Stack< T >::operator=(Stack< T > && rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      delete[] data_;
      capacity_ = rhs.capacity_;
      size_ = rhs.size_;
      data_ = rhs.data_;
      rhs.capacity_ = 0;
      rhs.size_ = 0;
      rhs.data_ = nullptr;
    }
    return *this;
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
  void Stack< T >::push(const T & data)
  {
    if (size_ == capacity_)
    {
      T * newData = nullptr;
      try
      {
        size_t newCapacity = 1;
        if (capacity_ != 0)
        {
          newCapacity = capacity_ * 2;
        }
        newData = new T[newCapacity];
        for (size_t i = 0; i < size_; ++i)
        {
          newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
      }
      catch (const std::exception &)
      {
        delete[] newData;
        throw;
      }
    }
    data_[size_++] = data;
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("ERROR: empty stack, cannot pop");
    }
    --size_;
  }

  template< typename T >
  T & Stack< T >::top()
  {
    return const_cast< T & >(static_cast< const Stack< T > & >(*this).top());
  }

  template< typename T >
  const T & Stack< T >::top() const
  {
    if (empty())
    {
      throw std::runtime_error("ERROR: empty stack, cannot top");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  void Stack< T >::swap(Stack< T > & rhs) noexcept
  {
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_, rhs.size_);
    std::swap(data_, rhs.data_);
  }
}

#endif
