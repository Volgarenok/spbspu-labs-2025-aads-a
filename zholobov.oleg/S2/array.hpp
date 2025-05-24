#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stdexcept>
#include <utility>

namespace zholobov {

  template < typename T >
  class Array {
    static constexpr size_t initial_capacity = 4;

  public:
    Array();
    Array(const Array& other);
    Array(Array&& other) noexcept;
    Array& operator=(const Array& other);
    Array& operator=(Array&& other) noexcept;
    ~Array();

    void push_back(const T& value);
    void push_back(T&& value);
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();
    void pop_front();
    void pop_back();
    bool empty() const;
    void clear() noexcept;
    void swap(Array& other) noexcept;

  private:
    void realloc();

    T* data_;
    size_t capacity_;
    size_t head_;
    size_t size_;
  };

}

template < typename T >
zholobov::Array< T >::Array():
  data_(nullptr),
  capacity_(0),
  head_(0),
  size_(0)
{}

template < typename T >
zholobov::Array< T >::Array(const Array& other):
  data_(nullptr),
  capacity_(0),
  head_(0),
  size_(0)
{
  if (other.size_ > 0) {
    try {
      data_ = new T[other.capacity_];
      capacity_ = other.capacity_;
      for (; size_ < other.size_; ++size_) {
        size_t pos = (other.head_ + size_) % other.capacity_;
        data_[size_] = other.data_[pos];
      }
    } catch (const std::exception& e) {
      clear();
      throw;
    }
  }
}

template < typename T >
zholobov::Array< T >::Array(Array&& other) noexcept:
  data_(nullptr),
  capacity_(0),
  head_(0),
  size_(0)
{
  swap(other);
}

template < typename T >
zholobov::Array< T >& zholobov::Array< T >::operator=(const Array& other)
{
  if (this != std::addressof(other)) {
    Array temp(other);
    swap(temp);
  }
  return *this;
}

template < typename T >
zholobov::Array< T >& zholobov::Array< T >::operator=(Array&& other) noexcept
{
  if (this != std::addressof(other)) {
    clear();
    swap(other);
  }
  return *this;
}

template < typename T >
zholobov::Array< T >::~Array()
{
  clear();
}

template < typename T >
void zholobov::Array< T >::push_back(const T& value)
{
  if (size_ == capacity_) {
    realloc();
  }
  size_t tail = (head_ + size_) % capacity_;
  data_[tail] = value;
  ++size_;
}

template < typename T >
void zholobov::Array< T >::push_back(T&& value)
{
  if (size_ == capacity_) {
    realloc();
  }
  size_t tail = (head_ + size_) % capacity_;
  data_[tail] = std::move(value);
  ++size_;
}

template < typename T >
const T& zholobov::Array< T >::front() const
{
  if (empty()) {
    throw std::out_of_range("Array is empty");
  }
  return data_[head_];
}

template < typename T >
T& zholobov::Array< T >::front()
{
  if (empty()) {
    throw std::out_of_range("Array is empty");
  }
  return data_[head_];
}

template < typename T >
const T& zholobov::Array< T >::back() const
{
  if (empty()) {
    throw std::out_of_range("Array is empty");
  }
  return data_[(head_ + size_ - 1) % capacity_];
}

template < typename T >
T& zholobov::Array< T >::back()
{
  if (empty()) {
    throw std::out_of_range("Array is empty");
  }
  return data_[(head_ + size_ - 1) % capacity_];
}

template < typename T >
void zholobov::Array< T >::pop_front()
{
  if (empty()) {
    throw std::out_of_range("Array is empty");
  }
  head_ = (head_ + 1) % capacity_;
  --size_;
}

template < typename T >
void zholobov::Array< T >::pop_back()
{
  if (empty()) {
    throw std::out_of_range("Array is empty");
  }
  --size_;
}

template < typename T >
bool zholobov::Array< T >::empty() const
{
  return size_ == 0;
}

template < typename T >
void zholobov::Array< T >::clear() noexcept
{
  size_ = 0;
  if (data_ != nullptr) {
    delete[] data_;
    data_ = nullptr;
    capacity_ = 0;
  }
}

template < typename T >
void zholobov::Array< T >::swap(Array& other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(capacity_, other.capacity_);
  std::swap(head_, other.head_);
  std::swap(size_, other.size_);
}

template < typename T >
void zholobov::Array< T >::realloc()
{
  size_t new_capacity = (capacity_ != 0) ? capacity_ * 2 : initial_capacity;
  T* new_data = nullptr;
  try {
    new_data = new T[new_capacity];
    for (size_t i = 0; i < size_; ++i) {
      size_t pos = (head_ + i) % capacity_;
      new_data[i] = std::move(data_[pos]);
    }
    delete[] data_;
    data_ = new_data;
    head_ = 0;
    capacity_ = new_capacity;
  } catch (...) {
    delete[] new_data;
    throw;
  }
}

#endif
