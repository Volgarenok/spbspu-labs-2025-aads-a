#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <utility>
#include <cstddef>
#include <stdexcept>
#include <cassert>

namespace aleksandrov
{
  constexpr size_t minQueueCapacity = 64;

  template< class T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue&);
    Queue(Queue&&) noexcept;
    ~Queue() noexcept;

    Queue& operator=(const Queue&);
    Queue& operator=(Queue&&) noexcept;

    T& front();
    const T& front() const;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    void clear() noexcept;
    void push(const T&);
    void push(T&&);
    template< class... Args >
    void emplace(Args&&...);
    void pop();
    void swap(Queue&) noexcept;

  private:
    T* data_;
    T* first_;
    size_t size_;
    size_t capacity_;

    T* copyData(const Queue&);
    void resize();
  };

  template< class T >
  Queue< T >::Queue():
    data_(static_cast< T* >(operator new(minQueueCapacity * sizeof(T)))),
    first_(data_),
    size_(0),
    capacity_(minQueueCapacity)
  {}

  template< class T >
  Queue< T >::Queue(const Queue& rhs):
    data_(copyData(rhs)),
    first_(rhs.empty() ? nullptr : data_ + std::distance(rhs.data_, rhs.first_)),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {}

  template< class T >
  Queue< T >::Queue(Queue&& rhs) noexcept:
    data_(std::exchange(rhs.data_, nullptr)),
    first_(std::exchange(rhs.first_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0))
  {}

  template< class T >
  Queue< T >::~Queue() noexcept
  {
    clear();
    operator delete(data_);
  }

  template< class T >
  Queue< T >& Queue< T >::operator=(const Queue& rhs)
  {
    Queue newQueue(rhs);
    swap(newQueue);
    return *this;
  }

  template< class T >
  Queue< T >& Queue< T >::operator=(Queue&& rhs) noexcept
  {
    Queue newQueue(std::move(rhs));
    swap(newQueue);
    return *this;
  }

  template< class T >
  T& Queue< T >::front()
  {
    assert(!empty());
    return *first_;
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    assert(!empty());
    return *first_;
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return !size_;
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  size_t Queue< T >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    while (!empty())
    {
      pop();
    }
  }

  template< class T >
  void Queue< T >::push(const T& el)
  {
    emplace(el);
  }

  template< class T >
  void Queue< T >::push(T&& el)
  {
    emplace(std::move(el));
  }

  template< class T >
  template< class... Args >
  void Queue< T >::emplace(Args&&... args)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    size_t sizeFromFirst = data_ + capacity_ - first_;
    if (size_ < sizeFromFirst)
    {
      new (first_ + size_) T(std::forward< Args >(args)...);
    }
    else
    {
      new (first_ - (capacity_ - size_)) T(std::forward< Args >(args)...);
    }
    ++size_;
  }

  template< class T >
  void Queue< T >::pop()
  {
    assert(!empty());
    (first_ ? first_++ : first_ = data_)->~T();
    if (empty())
    {
      first_ = data_;
    }
    --size_;
  }

  template< class T >
  void Queue< T >::swap(Queue& rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(first_, rhs.first_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
  }

  template< class T >
  T* Queue< T >::copyData(const Queue& rhs)
  {
    T* copy = static_cast< T* >(operator new(rhs.capacity_ * sizeof(T)));
    size_t i = 0;
    try
    {
      for (; i < rhs.size_; ++i)
      {
        new (copy + i) T(rhs.data_[i]);
      }
    }
    catch (const std::bad_alloc&)
    {
      for (size_t j = 0; j < i; ++j)
      {
        (copy + j)->~T();
      }
      operator delete(copy);
      throw;
    }
    return copy;
  }

  template< class T >
  void Queue< T >::resize()
  {
    size_t newCapacity = capacity_ * 2;
    T* newData = static_cast< T* >(operator new(newCapacity * sizeof(T)));
    size_t shift = std::distance(data_, first_);
    size_t i = shift;
    try
    {
      for (; i < size_ - shift; ++i)
      {
        new (newData + i) T(std::move_if_noexcept(data_[i]));
      }
      if (shift)
      {
        for (size_t j = 0; j < shift; ++j, ++i)
        {
          new (newData + i) T(std::move_if_noexcept(data_[j]));
        }
      }
    }
    catch (const std::bad_alloc&)
    {
      for (size_t j = 0; j < i; ++j)
      {
        newData[j].~T();
      }
      operator delete(newData);
      throw;
    }
    clear();
    operator delete(data_);
    data_ = newData;
    size_ = capacity_;
    capacity_ = newCapacity;
  }
}

#endif

