#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <algorithm>
#include <cassert>

namespace sveshnikov
{
  template < class T >
  class Queue
  {
  public:
    explicit Queue();
    Queue(const Queue &other);
    Queue(Queue &&other);
    ~Queue();
    Queue< T > &operator=(const Queue &other);
    Queue< T > &operator=(Queue &&other) noexcept;

    T &front() noexcept;
    const T &front() const noexcept;
    size_t getSize() const noexcept;
    bool empty() const noexcept;
    void push(const T &data);
    void push(T &&data);
    void pop();
    void swap(Queue &other) noexcept;

  private:
    size_t capacity_;
    T *data_;
    size_t size_;
    size_t first_;
    void resize();
    void reset();
    T *formNewData(size_t capacity) const;
  };

  template < class T >
  Queue< T >::Queue():
    capacity_(0),
    data_(nullptr),
    size_(0),
    first_(0)
  {}

  template < class T >
  Queue< T >::Queue(const Queue &other):
    capacity_(other.capacity_),
    data_(other.formNewData(other.capacity_)),
    size_(other.size_),
    first_(0)
  {}

  template < class T >
  Queue< T >::Queue(Queue &&other):
    capacity_(other.capacity_),
    data_(other.data_),
    size_(other.size_),
    first_(other.first_)
  {
    other.reset();
  }

  template < class T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }

  template < class T >
  Queue< T > &Queue< T >::operator=(const Queue &other)
  {
    if (this != std::addressof(other))
    {
      Queue rhs(other);
      swap(rhs);
    }
    return *this;
  }

  template < class T >
  Queue< T > &Queue< T >::operator=(Queue &&other) noexcept
  {
    if (this != std::addressof(other))
    {
      delete[] data_;
      reset();
      swap(other);
    }
    return *this;
  }

  template < class T >
  T &Queue< T >::front() noexcept
  {
    return const_cast< T & >(static_cast< const Queue< T > & >(*this).front());
    ;
  }

  template < class T >
  const T &Queue< T >::front() const noexcept
  {
    assert(!empty());
    return data_[first_];
  }

  template < class T >
  size_t Queue< T >::getSize() const noexcept
  {
    return size_;
  }

  template < class T >
  bool Queue< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class T >
  void Queue< T >::resize()
  {
    if (size_ == capacity_)
    {
      T *new_data = formNewData(capacity_ + 100);
      capacity_ += 100;
      first_ = 0;
      delete[] data_;
      data_ = new_data;
    }
  }

  template < class T >
  void Queue< T >::push(const T &data)
  {
    resize();
    data_[(first_ + size_) % capacity_] = data;
    size_++;
  }

  template < class T >
  void Queue< T >::push(T &&data)
  {
    resize();
    data_[(first_ + size_) % capacity_] = std::move(data);
    size_++;
  }

  template < class T >
  void Queue< T >::pop()
  {
    assert(!empty());
    size_--;
    first_ = (first_ + 1) % capacity_;
  }

  template < class T >
  void Queue< T >::swap(Queue &other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(first_, other.first_);
  }

  template < class T >
  void Queue< T >::reset()
  {
    capacity_ = 0;
    data_ = nullptr;
    size_ = 0;
    first_ = 0;
  }

  template < class T >
  T *Queue< T >::formNewData(size_t capacity) const
  {
    T *new_data = new T[capacity];
    for (size_t i = 0; i < size_; i++)
    {
      new_data[i] = data_[(first_ + i) % capacity_];
    }
    return new_data;
  }
}

#endif
