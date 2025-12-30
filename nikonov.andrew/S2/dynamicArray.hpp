#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP
#include <cstddef>
#include <stdexcept>
#include <cassert>
namespace nikonov
{
  constexpr size_t defaultCapacity = 10;
  constexpr size_t reallocScale = 2;

  template< typename T >
  class Array
  {
    T * data_;
    size_t size_;
    size_t capacity_;
    size_t start_;
    void reallocate(size_t newCap);
  public:
    Array();
    Array(const Array & copy);
    Array(Array && copy) noexcept;
    ~Array();

    Array & operator=(const Array & rhs);
    Array & operator=(Array && rhs) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    T & front() noexcept;
    const T & front() const noexcept;
    T & back() noexcept;
    const T & back() const noexcept;
    void push_back(const T & value);
    void push_back(T && value);
    template< typename... Args >
    void emplace(Args &&...args);
    void pop_front();
    void pop_back() noexcept;
    void swap(Array & x) noexcept;
  };
}

template< typename T >
void nikonov::Array< T >::reallocate(size_t newCap)
{
  if (newCap < size_)
  {
    throw std::runtime_error("ERROR: non-correct new capacity size");
  }
  T * tempArr = new T[newCap];
  try
  {
    for (size_t i = 0; i < size_; ++i)
    {
      tempArr[i] = std::move(data_[start_ + i]);
    }
  }
  catch(...)
  {
    delete[] tempArr;
    throw;
  }
  delete[] data_;
  data_ = tempArr;
  capacity_ = newCap;
  start_ = 0;
}

template< typename T >
nikonov::Array< T >::Array():
  data_(new T[defaultCapacity]),
  size_(0),
  capacity_(defaultCapacity),
  start_(0)
{}
template< typename T >
nikonov::Array< T >::Array(const Array & copy):
  data_(new T[copy.capacity_]),
  size_(copy.size_),
  capacity_(copy.capacity_),
  start_(0)
{
  try
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i] = copy.data_[copy.start_ + i];
    }
  }
  catch (...)
  {
    delete[] data_;
    throw;
  }
}
template< typename T >
nikonov::Array< T >::Array(Array && rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_),
  start_(rhs.start_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.start_ = 0;
  rhs.capacity_ = 0;
}
template< typename T >
nikonov::Array< T >::~Array()
{
  delete[] data_;
}

template< typename T >
bool nikonov::Array< T >::empty() const noexcept
{
  return (size_ == 0);
}
template< typename T >
size_t nikonov::Array< T >::size() const noexcept
{
  return size_;
}
template< typename T >
nikonov::Array< T > & nikonov::Array< T >::operator=(const Array & rhs)
{
  Array< T > copy{rhs};
  swap(*this, copy);
  return *this;
}

template< typename T >
nikonov::Array< T > & nikonov::Array< T >::operator=(Array && rhs) noexcept
{
  Array< T > copy{std::move(rhs)};
  swap(*this, copy);
  return *this;
}
template< typename T >
T & nikonov::Array< T >::front() noexcept
{
  assert(size_ != 0);
  return data_[start_];
}
template< typename T >
const T & nikonov::Array< T >::front() const noexcept
{
  assert(size_ != 0);
  return data_[start_];
}
template< typename T >
T & nikonov::Array< T >::back() noexcept
{
  assert(size_ != 0);
  return data_[start_ + size_ - 1];
}
template< typename T >
const T & nikonov::Array< T >::back() const noexcept
{
  assert(size_ != 0);
  return data_[start_ + size_ - 1];
}
template< typename T >
void nikonov::Array< T >::push_back(const T & value)
{
  emplace(value);
}
template< typename T >
void nikonov::Array< T >::push_back(T && value)
{
  emplace(std::forward< T >(value));
}
template< typename T >
template< typename... Args >
void nikonov::Array< T >::emplace(Args &&...args)
{
  if (start_ + size_ >= capacity_)
  {
    reallocate(capacity_ * reallocScale);
  }
  data_[start_ + size_] = T(std::forward<Args>(args)...);
  ++size_;
}
template< typename T >
void nikonov::Array< T >::pop_front()
{
  assert(size_ != 0);
  ++start_;
  --size_;
}
template< typename T >
void nikonov::Array< T >::pop_back() noexcept
{
  assert(size_ != 0);
  --size_;
}
template< typename T >
void nikonov::Array< T >::swap(Array & x) noexcept
{
  std::swap(size_, x.size_);
  std::swap(capacity_, x.capacity_);
  std::swap(data_, x.data_);
  std::swap(start_, x.start_);
}
#endif
