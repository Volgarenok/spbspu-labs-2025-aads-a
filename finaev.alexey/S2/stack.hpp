#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <utility>

namespace finaev
{
  template < class T >
  class Stack
  {
  public:
    Stack();
    void swap(Stack< T >&) noexcept;
    Stack(const Stack< T >& other);
    Stack< T >& operator=(const Stack< T >& other);
    Stack(Stack< T >&&) noexcept;
    Stack< T >& operator=(Stack<T>&&) noexcept;
    void addSize();
    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    void push(T rhs);
    void pop();
    T& top() const noexcept;
    ~Stack();
  private:
    T* data_;
    size_t size_;
    size_t capacity_;
  };

  template< class T >
  Stack< T >::Stack():
    data_(nullptr),
    size_(0),
    capacity_(20)
  {
    data_ = new T[capacity_];
  }

  template < class T >
  void Stack< T>::swap(Stack<T>& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template< class T >
  Stack< T >::Stack(const Stack< T >& other)
  {
    data_ = new T[other.capacity_];
    capacity_ = other.capacity_;
    size_ = other.size_;
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i] = other.data_[i];
    }
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& other)
  {
    if (this == &other)
    {
      return *this;
    }
    Stack< T > temp(other);
    swap(temp);
    return *this;
  }

  template < class T >
  Stack< T >::Stack(Stack< T >&& other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
  }

  template < class T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& other) noexcept
  {
    if (this == &other)
    {
      return *this;
    }
    swap(other);
    return *this;
  }

  template < class T >
  void Stack< T >::addSize()
  {
    const size_t newSize = capacity_ + 50;
    T* newArr = new T[newSize];
    for (size_t i = 0; i < size_; ++i)
    {
      newArr[i] = data_[i];
    }
    delete[] data_;
    capacity_ += 50;
    data_ = newArr;
  }

  template < class T >
  bool Stack< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template < class T >
  size_t Stack< T >::size() const noexcept
  {
    return size_;
  }

  template < class T >
  void Stack< T >::push(T rhs)
  {
    if (size_ == capacity_)
    {
      addSize();
    }
    data_[size_] = rhs;
    ++size_;
  }

  template < class T >
  T& Stack< T >::top() const noexcept
  {
    return data_[size_ - 1];
  }

  template < class T >
  void Stack< T >::pop()
  {
    --size_;
  }

  template< class T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }
}

#endif
