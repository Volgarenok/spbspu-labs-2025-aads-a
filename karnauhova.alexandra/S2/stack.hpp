#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <stdexcept>

namespace karnauhova
{
  template< typename T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack& rhs);
    Stack(Stack&& rhs);
    Stack< T >& operator=(const Stack< T >& rhs);
    ~Stack();

    bool empty() const;
    size_t size() const;

    void pop();
    void push(const T& val);
    T& top();
    const T& top() const;

    void swap(Stack< T >& other) noexcept;
  private:
    size_t size_;
    size_t count_element_;
    T* data_;
    void resize(size_t add_size);
  };

  template< typename T >
  Stack< T >::Stack():
    size_(5),
    count_element_(0),
    data_(new T[5])
  {}

  template< typename T >
  Stack< T >::Stack(const Stack& rhs):
    size_(rhs.size_),
    count_element_(rhs.count_element_),
    data_(new T[rhs.size_])
  {
    for (size_t i = 0; i < size_; i++)
    {
      data_[i] = rhs.data_[i];
    }
  }

  template< typename T >
  Stack< T >::Stack(Stack&& rhs):
    size_(rhs.size_),
    count_element_(rhs.count_element_),
    data_(rhs.data_)
  {
    rhs.size_ = 0;
    rhs.count_element_ = 0;
    rhs.data_ = nullptr;
  }

  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& rhs)
  {
    if (this != &rhs)
    {
      Stack< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }

  template< typename T >
  bool Stack< T >::empty() const
  {
    return count_element_ == 0;
  }

  template< typename T >
  size_t Stack< T >::size() const
  {
    return count_element_;
  }

  template< typename T >
  void Stack< T >::resize(size_t add_size)
  {
    T* new_data = new T[size_ + add_size];
    for (size_t i = 0; i < size_; i++)
    {
      new_data[i] = data_[i];
    }
    size_ += add_size;
    delete[] data_;
    data_ = new_data;
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty stack for pop");
    }
    count_element_--;
  }

  template< typename T >
  void Stack< T >::push(const T& val)
  {
    if (size_ == count_element_)
    {
      resize(1);
    }
    data_[count_element_++] = val;
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    if (empty())
    {
      throw std::logic_error("empty stack for top");
    }
    return data_[count_element_ - 1];
  }

  template< typename T >
  T& Stack< T >::top()
  {
    if (empty())
    {
      throw std::logic_error("empty stack for top");
    }
    return data_[count_element_ - 1];
  }

  template< typename T >
  void Stack< T >::swap(Stack< T >& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(count_element_, other.count_element_);
    std::swap(data_, other.data_);
  }
}

#endif
