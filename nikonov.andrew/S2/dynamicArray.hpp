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
    T * arr_;
    size_t size_;
    size_t capacity_;
    void reallocate(size_t newCap);
  public:
    Array();
    Array(const Array & copy);
    Array(Array && copy);
    ~Array();
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;//временная ф-я для тестирования
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
  if (capacity_ >= newCap)
  {
    return;
  }
  T * tempArr = new T[newCap];
  for (size_t i = 0; i < size_; ++i)
  {
    tempArr[i] = arr_[i];
  }
  delete[] arr_;
  arr_ = tempArr;
  capacity_ = newCap;
}

template< typename T >
nikonov::Array< T >::Array():
  arr_(new T[defaultCapacity]),
  size_(0),
  capacity_(defaultCapacity)
{}
template< typename T >
nikonov::Array< T >::Array(const Array & copy):
  arr_(new T[copy.capacity_]),
  size_(copy.size_),
  capacity_(copy.capacity_)
{
  for (size_t i = 0; i < size_; ++i)
  {
    arr_[i] = copy.arr_[i];
  }
}
template< typename T >
nikonov::Array< T >::Array(Array && copy):
  arr_(copy.arr_),
  size_(copy.size_),
  capacity_(copy.capacity_)
{
  copy.arr_ = nullptr;
  copy.size_ = 0;
}
template< typename T >
nikonov::Array< T >::~Array()
{
  delete[] arr_;
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
size_t nikonov::Array< T >::capacity() const noexcept
{
  return capacity_;
}
template< typename T >
T & nikonov::Array< T >::front() noexcept
{
  assert(size_ != 0);
  return *arr_;
}
template< typename T >
const T & nikonov::Array< T >::front() const noexcept
{
  assert(size_ != 0);
  return *arr_;
}
template< typename T >
T & nikonov::Array< T >::back() noexcept
{
  assert(size_ != 0);
  return arr_[size_ - 1];
}
template< typename T >
const T & nikonov::Array< T >::back() const noexcept
{
  assert(size_ != 0);
  return arr_[size_ - 1];
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
  if (size_ == capacity_)
  {
    reallocate(capacity_ * reallocScale);
  }
  T data = T(std::forward< Args >(args)...);
  arr_[size_] = data;
  ++size_;
}
template< typename T >
void nikonov::Array< T >::pop_front()
{
  assert(size_ != 0);
  for (size_t i = 0; i < size_ - 1; ++i)
  {
    arr_[i] = arr_[i + 1];
  }
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
  std::swap(arr_, x.arr_);
}
#endif
