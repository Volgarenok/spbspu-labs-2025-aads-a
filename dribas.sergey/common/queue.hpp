#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>

namespace dribas
{
  template< class T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue< T >&);
    Queue(Queue< T >&&) noexcept;
    ~Queue();

    Queue& operator=(const Queue< T >&);
    Queue& operator=(Queue< T >&&) noexcept;

    T& back();
    const T& back() const;

    void push(const T&);
    void pop();
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(Queue< T >&) noexcept;

  private:
    size_t size_;
    size_t capacity_;
    size_t head_;
    size_t tail_;
    T* queue_;
    void resize(size_t);
  };

  template< class T >
  Queue< T >::Queue() :
    size_(0),
    capacity_(0),
    head_(0),
    tail_(0),
    queue_(nullptr)
  {}

  template< class T >
  Queue< T >::Queue(const Queue< T >& other):
    size_(other.size_),
    capacity_(other.size_),
    head_(0),
    tail_(0),
    queue_(size_ ? new T[capacity_] : nullptr)
  {
    try {
      for (size_t i = 0; i < size_; ++i) {
        queue_[i] = other.queue_[(other.head_ + i) % other.capacity_];
      }
      tail_ = size_;
    } catch (const std::exception&) {
      delete[] queue_;
      throw;
    }
  }

  template< class T >
  Queue< T >::Queue(Queue< T >&& other) noexcept:
    size_(other.size_),
    capacity_(other.capacity_),
    head_(other.head_),
    tail_(other.tail_),
    queue_(other.queue_)
  {
    other.size_ = 0;
    other.capacity_ = 0;
    other.head_ = 0;
    other.tail_ = 0;
    other.queue_ = nullptr;
  }

  template< class T >
  Queue< T >::~Queue()
  {
    delete[] queue_;
  }

  template< class T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& other)
  {
    if (this != std::addressof(other)) {
      Queue< T > tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& other) noexcept
  {
    if (this != std::addressof(other)) {
      Queue< T > tmp(std::move(other));
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  void Queue< T >::resize(size_t new_capacity)
  {
    T* new_queue = new T[new_capacity];
    try {
      for (size_t i = 0; i < size_; ++i) {
        new_queue[i] = std::move(queue_[(head_ + i) % capacity_]);
      }
      delete[] queue_;
      queue_ = new_queue;
      capacity_ = new_capacity;
      head_ = 0;
      tail_ = size_;
    } catch (const std::exception&) {
      delete[] new_queue;
      throw;
    }
  }

  template< class T >
  void Queue< T >::push(const T& value)
  {
    if (size_ == capacity_) {
      resize(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    queue_[tail_] = value;
    tail_ = (tail_ + 1) % capacity_;
    ++size_;
  }

  template< class T >
  void Queue< T >::pop()
  {
    head_ = (head_ + 1) % capacity_;
    --size_;
    if (capacity_ > 4 && size_ < capacity_ / 4) {
      resize(capacity_ / 2);
    }
  }

  template< class T >
  T& Queue< T >::back()
  {

    return queue_[head_];
  }

  template< class T >
  const T& Queue< T >::back() const
  {
    return queue_[head_];
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void Queue< T >::swap(Queue< T >& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(queue_, other.queue_);
  }
}

#endif
