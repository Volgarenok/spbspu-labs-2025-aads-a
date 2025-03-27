#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <utility>

namespace finaev
{
  template < class T >
  class Queue
  {
  public:
    Queue();
    void swap(Queue< T >&) noexcept;
    Queue(const Queue< T >&);
    Queue< T >& operator=(const Queue< T >&);
    Queue(Queue< T >&&) noexcept;
    Queue< T >& operator=(Queue< T >&&) noexcept;
    void addSize();
    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    void push(T rhs);
    T& top() const noexcept;
    void pop();
    ~Queue();
  private:
    T* data_;
    size_t capacity_;
    size_t size_;
  };

  template < class T >
  Queue< T >::Queue():
    data_(nullptr),
    capacity_(50),
    size_(0)
  {
    data_ = new T[capacity_];
  }

  template < class T >
  void Queue< T >::swap(Queue< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
  }

  template < class T >
  Queue< T >::Queue(const Queue< T >& other)
  {
    data_ = new T[other.capacity_];
    capacity_ = other.capacity_;
    size_ = other.size_;
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i] = other.data_[i];
    }
  }

  template < class T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& other)
  {
    if (this == &other)
    {
      return *this;
    }
    Queue< T > temp(other);
    swap(temp);
    return *this;
  }

  template < class T >
  Queue< T >::Queue(Queue< T >&& other) noexcept:
    data_(other.data_),
    capacity_(other.capacity_),
    size_(other.size_)
  {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
  }

  template < class T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& other) noexcept
  {
    if (this == &other)
    {
      return *this;
    }
    swap(other);
    return *this;
  }

  template < class T >
  void Queue< T >::addSize()
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
  bool Queue< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template < class T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }

  template < class T >
  void Queue< T >::push(T rhs)
  {
    if (size_ == capacity_)
    {
      addSize();
    }
    data_[size_] = rhs;
    ++size_;
  }

  template < class T >
  T& Queue< T >::top() const noexcept
  {
    return data_[0];
  }

  template < class T >
  void Queue< T >::pop()
  {
    for (size_t i = 1; i < size_; ++i)
    {
      data_[i - 1] = std::move(data_[i]);
    }
    --size_;
  }

  template < class T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }
}

#endif
