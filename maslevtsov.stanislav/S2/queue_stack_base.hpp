#ifndef QUEUE_STACK_BASE_HPP
#define QUEUE_STACK_BASE_HPP

#include <utility>

namespace maslevtsov {
  template< class T, bool is_pop_front >
  class QueueStackBase
  {
  public:
    QueueStackBase() noexcept;
    QueueStackBase(const QueueStackBase& rhs);
    QueueStackBase(QueueStackBase&& rhs) noexcept;
    ~QueueStackBase();

    QueueStackBase& operator=(const QueueStackBase& rhs);
    QueueStackBase& operator=(QueueStackBase&& rhs) noexcept;

    T& operator[](std::size_t index) noexcept;
    const T& operator[](std::size_t index) const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;
    void swap(QueueStackBase& other) noexcept;

  private:
    T* data_;
    std::size_t size_, first_, capacity_;

    template< class U >
    void push_impl(U&& value);
    void expand_data(std::size_t new_capacity);
  };

  template< class T, bool is_pop_front >
  QueueStackBase< T, is_pop_front >::QueueStackBase() noexcept:
    data_(nullptr),
    size_(0),
    first_(0),
    capacity_(0)
  {}

  template< class T, bool is_pop_front >
  QueueStackBase< T, is_pop_front >::QueueStackBase(const QueueStackBase& rhs):
    data_(new T[rhs.capacity_]),
    size_(rhs.size_),
    first_(0),
    capacity_(rhs.capacity_)
  {
    try {
      if (is_pop_front) {
        for (std::size_t i = 0, j = rhs.first_; i != capacity_; ++i, j = (j + 1) % capacity_) {
          data_[i] = rhs.data_[j];
        }
      } else {
        for (std::size_t i = 0; i != capacity_; ++i) {
          data_[i] = rhs.data_[i];
        }
      }
    } catch (...) {
      delete[] data_;
      throw;
    }
  }

  template< class T, bool is_pop_front >
  QueueStackBase< T, is_pop_front >::QueueStackBase(QueueStackBase&& rhs) noexcept:
    data_(std::exchange(rhs.data_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    first_(std::exchange(rhs.first_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0))
  {}

  template< class T, bool is_pop_front >
  QueueStackBase< T, is_pop_front >::~QueueStackBase()
  {
    delete[] data_;
  }

  template< class T, bool is_pop_front >
  QueueStackBase< T, is_pop_front >& QueueStackBase< T, is_pop_front >::operator=(const QueueStackBase& rhs)
  {
    QueueStackBase< T, is_pop_front > copied(rhs);
    swap(copied);
    return *this;
  }

  template< class T, bool is_pop_front >
  QueueStackBase< T, is_pop_front >& QueueStackBase< T, is_pop_front >::operator=(QueueStackBase&& rhs) noexcept
  {
    QueueStackBase< T, is_pop_front > moved(std::move(rhs));
    swap(rhs);
    return *this;
  }

  template< class T, bool is_pop_front >
  T& QueueStackBase< T, is_pop_front >::operator[](std::size_t index) noexcept
  {
    return data_[is_pop_front ? ((first_ + index) % capacity_) : (size_ - 1)];
  }

  template< class T, bool is_pop_front >
  const T& QueueStackBase< T, is_pop_front >::operator[](std::size_t index) const noexcept
  {
    return data_[is_pop_front ? ((first_ + index) % capacity_) : (size_ - 1)];
  }

  template< class T, bool is_pop_front >
  bool QueueStackBase< T, is_pop_front >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T, bool is_pop_front >
  std::size_t QueueStackBase< T, is_pop_front >::size() const noexcept
  {
    return size_;
  }

  template< class T, bool is_pop_front >
  void QueueStackBase< T, is_pop_front >::push(const T& value)
  {
    push_impl(value);
  }

  template< class T, bool is_pop_front >
  void QueueStackBase< T, is_pop_front >::push(T&& value)
  {
    push_impl(std::move(value));
  }

  template< class T, bool is_pop_front >
  void QueueStackBase< T, is_pop_front >::pop() noexcept
  {
    if (is_pop_front) {
      first_ = (first_ + 1) % capacity_;
    }
    --size_;
  }

  template< class T, bool is_pop_front >
  void QueueStackBase< T, is_pop_front >::swap(QueueStackBase& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(first_, other.first_);
    std::swap(capacity_, other.capacity_);
  }

  template< class T, bool is_pop_front >
  template< class U >
  void QueueStackBase< T, is_pop_front >::push_impl(U&& value)
  {
    if (size_ == capacity_) {
      expand_data(size_ * 2 + 1);
    }
    data_[size_++] = std::forward< U >(value);
  }

  template< class T, bool is_pop_front >
  void QueueStackBase< T, is_pop_front >::expand_data(std::size_t new_capacity)
  {
    T* new_data = new T[new_capacity];
    try {
      if (is_pop_front) {
        for (std::size_t i = 0, j = first_; i != capacity_; ++i, j = (j + 1) % capacity_) {
          new_data[i] = data_[j];
        }
      } else {
        for (std::size_t i = 0; i != capacity_; ++i) {
          new_data[i] = data_[i];
        }
      }
    } catch (...) {
      delete[] new_data;
      throw;
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
    first_ = 0;
  }
}

#endif
