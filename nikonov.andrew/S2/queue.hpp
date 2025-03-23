#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <stdexcept>
#include <cassert>
namespace nikonov
{
  constexpr double reallocScale_ = 1.5; 

  template< typename T >
  class Queue
  {
    T ** arr_;
    size_t size_;
    size_t capacity_;
    void clear(size_t cnt) noexcept;
    void reallocate(size_t newCap);
  public:
    Queue();
    Queue(const Queue & copy);
    Queue(Queue && copy);
    ~Queue();

    bool empty() const noexcept;
    size_t size() const noexcept;
    T & front() noexcept;
    const T & front() const noexcept;
    T & back() noexcept;
    const T & back() const noexcept;
    void push(const T & val);
    void push(T && val);
    template< typename... Args >
    void emplace(Args &&... args);
    void pop() noexcept;
    void swap(Queue & x) noexcept;
  };
}

template< typename T >
void nikonov::Queue< T >::clear(size_t cnt) noexcept
{
  for (; cnt > 0; --cnt)
  {
    delete arr_[cnt - 1];
  }
  size_= 0;
}
template< typename T >
void nikonov::Queue< T >::reallocate(size_t newCap)
{
  if (capacity_ >= newCap)
  {
    return;
  }
  auto tempArr = new T * [newCap];
  for (size_t i = 0; i < size_; ++i)
  {
    tempArr[i] = arr_[i];
  }
  delete[] arr_;
  arr_ = tempArr;
  capacity_ = newCap;
}

template< typename T >
nikonov::Queue< T >::Queue():
  arr_(nullptr), size_(0), capacity_(0)
{}
template< typename T >
nikonov::Queue< T >::Queue(const Queue & copy):
  size_(copy.size_), capacity_(copy.capacity_)
{
  reallocate(capacity_);
  size_t cnt = 0;
  try
  {
    for (; cnt < size_; )
    {
      arr_[cnt] = new T(copy.arr_[cnt]);
    }
  }
  catch(const std::bad_alloc& e)
  {
    clear(cnt);
  }
}
template< typename T >
nikonov::Queue< T >::Queue(Queue && copy):
  arr_(copy.arr_), size_(copy.size_), capacity_(copy.capacity_)
{
  copy.arr = nullptr;
  copy.size = 0;
}
template< typename T >
nikonov::Queue< T >::~Queue()
{
  clear(size_);
  delete[] arr_;
}

template< typename T >
bool nikonov::Queue< T >::empty() const noexcept
{
  return size_;
}
template< typename T >
size_t nikonov::Queue< T >::size() const noexcept
{
  return size_;
}
template< typename T >
T & nikonov::Queue< T >::front() noexcept
{
  assert(size_ != 0);
  return *arr_;
}
template< typename T >
const T & nikonov::Queue< T >::front() const noexcept
{
  assert(size_ != 0);
  return *arr_;
}
template< typename T >
T & nikonov::Queue< T >::back() noexcept
{
  assert(size_ != 0);
  return arr_[size - 1];
}
template< typename T >
const T & nikonov::Queue< T >::back() const noexcept
{
  assert(size_ != 0);
  return arr_[size - 1];
}
template< typename T >
void nikonov::Queue< T >::push(const T & val)
{
  push(std::move(val));
}
template< typename T >
void nikonov::Queue< T >::push(T && val)
{
  if (size_ == capacity_)
  {
    reallocate(capacity_ * reallocScale_);
  }
  arr_[size] = new T(val);
  ++size;
}
template< typename T >
template< typename... Args >
void nikonov::Queue< T >::emplace(Args &&...args) //Args && - паттерн раскрытия пакета аргументов
{
}
template< typename T >
void nikonov::Queue< T >::pop() noexcept
{
  assert(size_ != 0);
  delete *arr_;
  --size_;
}

#endif
