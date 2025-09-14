#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <algorithm>

namespace sveshnikov
{
  template< class T >
  class Array
  {
  public:
    Array();
    Array(const Array &other);
    Array(Array &&other) noexcept;
    explicit Array(size_t capacity);
    ~Array();
    Array< T > &operator=(const Array &other);
    Array< T > &operator=(Array &&other) noexcept;

    T &front();
    const T &front() const;
    T &back();
    const T &back() const;

    size_t getSize() const noexcept;
    bool empty() const noexcept;

    void push_back(const T &val);
    void push_back(T &&val);
    void pop_back() noexcept;
    void pop_front() noexcept;

    T &operator[](size_t n);
    const T &operator[](size_t n) const;
    T &at(size_t n);
    const T &at(size_t n) const;

    void swap(Array &other) noexcept;

  private:
    size_t capacity_;
    T *data_;
    size_t size_;
    size_t first_;

    void extend();
    T *formNewData(size_t capacity) const;
    void reset();
  };

  template< class T >
  Array< T >::Array():
    capacity_(0),
    data_(nullptr),
    size_(0),
    first_(0)
  {}

  template< class T >
  Array< T >::Array(const Array &other):
    capacity_(other.capacity_),
    data_(other.formNewData(other.capacity_)),
    size_(other.size_),
    first_(0)
  {}

  template< class T >
  Array< T >::Array(Array &&other) noexcept:
    capacity_(other.capacity_),
    data_(other.data_),
    size_(other.size_),
    first_(other.first_)
  {
    other.reset();
  }

  template< class T >
  Array< T >::Array(size_t capacity):
    capacity_(capacity),
    data_((capacity != 0) ? new T[capacity]{} : nullptr),
    size_(0),
    first_(0)
  {}

  template< class T >
  Array< T >::~Array()
  {
    delete[] data_;
  }

  template< class T >
  Array< T > &Array< T >::operator=(const Array &other)
  {
    if (this != std::addressof(other))
    {
      Array rhs(other);
      swap(rhs);
    }
    return *this;
  }

  template< class T >
  Array< T > &Array< T >::operator=(Array &&other) noexcept
  {
    if (this != std::addressof(other))
    {
      delete[] data_;
      reset();
      swap(other);
    }
    return *this;
  }

  template< class T >
  const T &Array< T >::front() const
  {
    assert(!empty());
    return data_[first_];
  }

  template< class T >
  T &Array< T >::front()
  {
    return const_cast< T & >(static_cast< const Array< T > & >(*this).front());
  }

  template< class T >
  const T &Array< T >::back() const
  {
    assert(!empty());
    return data_[(first_ + size_ - 1) % capacity_];
  }

  template< class T >
  T &Array< T >::back()
  {
    return const_cast< T & >(static_cast< const Array< T > & >(*this).back());
  }

  template< class T >
  size_t Array< T >::getSize() const noexcept
  {
    return size_;
  }

  template< class T >
  bool Array< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  void Array< T >::push_back(const T &val)
  {
    extend();
    data_[(first_ + size_) % capacity_] = val;
    size_++;
  }

  template< class T >
  void Array< T >::push_back(T &&val)
  {
    extend();
    data_[(first_ + size_) % capacity_] = std::move(val);
    size_++;
  }

  template< class T >
  void Array< T >::pop_front() noexcept
  {
    assert(!empty());
    size_--;
    first_ = empty() ? 0 : (first_ + 1) % capacity_;
  }

  template< class T >
  void Array< T >::pop_back() noexcept
  {
    assert(!empty());
    size_--;
    first_ = empty() ? 0 : first_;
  }

  template< class T >
  T &Array< T >::operator[](size_t n)
  {
    return const_cast< T & >(static_cast< const Array< T > & >(*this)[n]);
  }

  template< class T >
  const T &Array< T >::operator[](size_t n) const
  {
    return data_[(first_ + n) % capacity_];
  }

  template< class T >
  T &Array< T >::at(size_t n)
  {
    return const_cast< T & >(static_cast< const Array< T > & >(*this).at(n));
  }

  template< class T >
  const T &Array< T >::at(size_t n) const
  {
    if (n >= size_)
    {
      throw std::out_of_range("Array::at");
    }
    return data_[(first_ + n) % capacity_];
  }

  template< class T >
  void Array< T >::swap(Array &other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(first_, other.first_);
  }

  template< class T >
  T *Array< T >::formNewData(size_t capacity) const
  {
    T *new_data = new T[capacity];
    for (size_t i = 0; i < size_; i++)
    {
      new_data[i] = data_[(first_ + i) % capacity_];
    }
    return new_data;
  }

  template< class T >
  void Array< T >::extend()
  {
    if (size_ < capacity_)
    {
      return;
    }
    T *new_data = formNewData(capacity_ + 100);
    capacity_ += 100;
    first_ = 0;
    delete[] data_;
    data_ = new_data;
  }

  template< class T >
  void Array< T >::reset()
  {
    capacity_ = 0;
    data_ = nullptr;
    size_ = 0;
    first_ = 0;
  }
}

#endif
