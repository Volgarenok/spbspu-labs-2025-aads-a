#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <utility>

namespace lebedev
{
  template< class T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack< T >&);
    Stack(Stack< T >&&) noexcept;
    Stack< T >& operator=(const Stack< T >& other);
    Stack< T >& operator=(Stack< T >&& other) noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    void push(const T& rhs);
    void pop();
    T& top() noexcept;
    void swap(Stack< T >& other) noexcept;
    ~Stack();
  private:
    void addSize();
    size_t capacity_;
    size_t size_;
    T* data_;
  };

  template< class T >
  Stack< T >::Stack():
    capacity_(0),
    size_(0),
    data_(new T[capacity_])
  {}

  template< class T >
  Stack< T >::Stack(const Stack< T >& other):
    capacity_(other.capacity_),
    size_(other.size_),
    data_(new T[capacity_])
  {
    if (capacity_ > 0)
    {
      try
      {
        for (size_t i = 0; i < size_; ++i)
        {
          data_[i] = other.data_[i];
        }
      }
      catch (...)
      {
        delete[] data_;
        data_ = nullptr;
        capacity_ = 0;
        size_ = 0;
        throw;
      }
    }
  }

  template < class T >
  Stack< T >::Stack(Stack< T >&& other) noexcept:
    capacity_(other.capacity_),
    size_(other.size_),
    data_(other.data_)
  {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    Stack< T > temp(other);
    swap(temp);
    return *this;
  }

  template < class T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  template < class T >
  void Stack< T >::addSize()
  {
    const size_t newCapacity = (capacity_ == 0) ? 10 : capacity_ * 2;
    T* newArr = nullptr;
    try
    {
      newArr = new T[newCapacity];
      for (size_t i = 0; i < size_; ++i)
      {
        newArr[i] = std::move(data_[i]);
      }
    }
    catch(...)
    {
      delete[] newArr;
      throw;
    }
    delete[] data_;
    capacity_ = newCapacity;
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
  void Stack< T >::push(const T& rhs)
  {
    if (size_ == capacity_)
    {
      addSize();
    }
    data_[size_] = rhs;
    ++size_;
  }

  template < class T >
  T& Stack< T >::top() noexcept
  {
    return data_[size_ - 1];
  }

  template < class T >
  void Stack< T >::pop()
  {
    --size_;
  }

  template < class T >
  void Stack< T>::swap(Stack<T>& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template< class T >
  Stack< T >::~Stack()
  {
    delete[] data_;
    data_ = nullptr;
  }
}
#endif
