#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace gavrilova {
  template < class T >
  class Queue {
  public:
    Queue();
    Queue(const Queue< T >& other);
    Queue(Queue< T >&& other);
    ~Queue();

    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other);

    void push(const T& value);
    void push(T&& value);

    T& front();
    T& back();
    void pop();
    bool empty() const;
    size_t size() const;
    void swap(Queue& other) noexcept;

  private:
    size_t size_;
    size_t capacity_;
    T* queue_;

    void resize();
  };

  template < class T >
  Queue< T >::Queue():
    size_(0),
    capacity_(50),
    queue_(new T[capacity_])
  {}

  template < class T >
  Queue< T >::Queue(const Queue< T >& other):
    size_(other.size_),
    capacity_(other.capacity_),
    queue_(new T[capacity_])
  {
    for  (size_t i = 0; i < size_; ++i) {
      queue_[i] = other.queue_[i];
    }
  }

  template < class T >
  Queue< T >::Queue(Queue<T>&& other):
    size_(other.size_),
    capacity_(other.capacity_),
    queue_(other.queue_)
  {
    other.size_ = 0;
    other.capacity_ = 0;
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
      Queue cpy(other);
      swap(cpy);
    }
    return *this;
  }

  template < class T >
  Queue< T >& Queue< T >::operator=(Queue&& other)
  {
    if (this != std::addressof(other)) {
      swap(other);
      delete[] other.queue_;
    }
    return *this;
  }

  template < class T >
  void Queue< T >::push(const T& value)
  {
    if (size_ == capacity_) {
      resize();
    }
    queue_[size_++] = value;
  }

  template < class T >
  void Queue< T >::push(T&& value)
  {
    if (size_ == capacity_) {
      resize();
    }
    queue_[size_++] = std::move(value);
  }

  template < class T >
  T& Queue< T >::front()
  {
    return queue_[0];
  }

  template < class T >
  T& Queue< T >::back()
  {
    return queue_[size_ - 1];
  }

  template < class T >
  void Queue< T >::pop()
  {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    for (size_t i = 1; i < size_; ++i) {
      queue_[i-1] = std::move(queue_[i]);
    }
    --size_;
  }

  template < class T >
  bool Queue< T >::empty() const
  {
    return !size_;
  }

  template < class T >
  size_t Queue< T >::size() const
  {
    return size_;
  }

  template < class T >
  void Queue< T >::swap(Queue& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(queue_, other.queue_);
  }

  template < class T >
  void Queue< T >::resize()
  {
    T* new_queue = new T[capacity_+50];
    for (size_t i = 0; i < size_; ++i) {
      new_queue[i] = queue_[i];
    }
    delete[] queue_;
    queue_ = new_queue;
    capacity_ += 50;
  }
}

#endif
