#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <utility>
#include <exception>

namespace lebedev
{
  template< class T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue< T >&);
    Queue(Queue< T >&&) noexcept;
    Queue< T >& operator=(const Queue< T >&);
    Queue< T >& operator=(Queue< T >&&) noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    void push(const T& rhs);
    T& top() noexcept;
    void pop();
    void swap(Queue< T >&) noexcept;
    ~Queue();
  private:
    void addSize();
    size_t capacity_;
    size_t size_;
    size_t head_;
    size_t tail_;
    T* data_;
  };

  template< class T >
  Queue< T >::Queue():
    capacity_(0),
    size_(0),
    head_(0),
    tail_(0),
    data_(new T[capacity_])
  {}

  template< class T >
  Queue< T >::Queue(const Queue< T >& other):
    capacity_(other.capacity_),
    size_(other.size_),
    head_(other.head_),
    tail_(other.tail_),
    data_(new T[other.capacity_])
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
  Queue< T >::Queue(Queue< T >&& other) noexcept:
    capacity_(other.capacity_),
    size_(other.size_),
    head_(other.head_),
    tail_(other.tail_),
    data_(other.data_)
  {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
    other.head_ = 0;
    other.tail_ = 0;
  }

  template < class T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    Queue< T > temp(other);
    swap(temp);
    return *this;
  }

  template < class T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      head_ = other.head_;
      tail_ = other.tail_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
      other.tail_ = 0;
      other.head_ = 0;
    }
    return *this;
  }

  template < class T >
  void Queue< T >::addSize()
  {
    const size_t newCapacity = (capacity_ == 0) ? 10 : capacity_ * 2;
    T* newArr = nullptr;
    try
    {
      newArr = new T[newCapacity];
      for (size_t i = 0; i < size_; ++i)
      {
        newArr[i] = data_[i];
      }
    }
    catch(const std::bad_alloc& e)
    {
      delete[] newArr;
      throw;
    }
    delete[] data_;
    capacity_ = newCapacity;
    data_ = newArr;
    head_ = 0;
    tail_ = size_;
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
  void Queue< T >::push(const T& rhs)
  {
    if (size_ == capacity_)
    {
      addSize();
    }
    data_[tail_] = rhs;
    ++tail_;
    ++size_;
  }

  template < class T >
  T& Queue< T >::top() noexcept
  {
    return data_[head_];
  }

  template< class T >
  void Queue< T >::pop()
  {
    ++head_;
    --size_;
  }

  template < class T >
  void Queue< T >::swap(Queue< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
  }

  template < class T >
  Queue< T >::~Queue()
  {
    delete[] data_;
    data_ = nullptr;
  }
}
#endif
