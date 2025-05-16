#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <memory>
#include <utility>
#include <stdexcept>

namespace dribas
{
  template < class T >
  class Queue {
  public:
    Queue();
    Queue(const Queue< T >& other);
    Queue(Queue< T >&& other);
    ~Queue();

    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other);

    T& front();
    const T& front() const;

    void push(const T& value);
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(Queue& other) noexcept;

  private:
    size_t size_;
    T* queue_;
  };

  template < class T >
  Queue< T >::Queue():
    size_(0),
    queue_(nullptr)
  {}

  template < class T >
  Queue< T >::Queue(const Queue< T >& other):
    size_(other.size_),
    queue_(size_ ? new T[size_] : nullptr)
  {
    for (size_t i = 0; i < size_; ++i) {
      queue_[i] = other.queue_[i];
    }
  }

  template < class T >
  Queue< T >::Queue(Queue<T>&& other):
    size_(other.size_),
    queue_(other.queue_)
  {
    other.size_ = 0;
    other.queue_ = nullptr;
  }

  template < class T >
  Queue< T >::~Queue()
  {
    delete[] queue_;
  }

  template < class T >
  Queue< T >& Queue< T >::operator=(const Queue& other)
  {
    if (this != std::addressof(other)) {
      Queue tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template < class T >
  Queue< T >& Queue< T >::operator=(Queue&& other)
  {
    if (this != std::addressof(other)) {
      delete[] queue_;
      size_ = other.size_;
      queue_ = other.queue_;
      other.size_ = 0;
      other.queue_ = nullptr;
    }
    return *this;
  }

  template < class T >
  void Queue< T >::push(const T& value)
  {
    T* new_queue = new T[size_ + 1];
    for (size_t i = 0; i < size_; ++i) {
      new_queue[i + 1] = queue_[i];
    }
    delete[] queue_;
    queue_ = new_queue;
    queue_[0] = value;
    size_++;
  }

  template < class T >
  void Queue< T >::pop() noexcept
  {
    size_--;
  }

  template < class T >
  T& Queue< T >::front()
  {
    return queue_[size_ - 1];
  }

  template < class T >
  const T& Queue< T >::front() const
  {
    return queue_[size_ - 1];
  }

  template < class T >
  bool Queue< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }

  template < class T >
  void Queue< T >::swap(Queue& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(queue_, other.queue_);
  }
}

#endif
