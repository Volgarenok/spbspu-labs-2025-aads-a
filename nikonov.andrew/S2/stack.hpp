#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <stdexcept>
#include <cassert>
namespace nikonov
{
  constexpr size_t defaultCapacity = 10;
  constexpr double reallocScale = 1.5; 

  template< typename T >
  class Stack
  {
    T ** arr_;
    size_t size_;
    size_t capacity_;
    void clear(size_t cnt) noexcept;
    void reallocate(size_t newCap);
  public:
    Stack();
    Stack(const Stack & copy);
    Stack(Stack && copy);
    ~Stack();

    bool empty() const noexcept;
    size_t size() const noexcept;
    T & top() noexcept;
    const T & top() const noexcept;
    void push(const T & val);
    void push(T && val);
    template< typename... Args >
    void emplace(Args &&... args);
    void pop() noexcept;
  };
}

template< typename T >
void nikonov::Stack< T >::clear(size_t cnt) noexcept
{
  for (; cnt > 0; --cnt)
  {
    delete arr_[cnt - 1];
  }
  size_= 0;
}
template< typename T >
void nikonov::Stack< T >::reallocate(size_t newCap)
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
nikonov::Stack< T >::Stack():
  arr_(nullptr),
  size_(0),
  capacity_(defaultCapacity)
{}
template< typename T >
nikonov::Stack< T >::Stack(const Stack & copy):
  arr_(new T * [copy.capacity_]),
  size_(copy.size_),
  capacity_(copy.capacity_)
{
  size_t cnt = 0;
  try
  {
    for (; cnt < size_; ++cnt)
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
nikonov::Stack< T >::Stack(Stack && copy):
  arr_(copy.arr_),
  size_(copy.size_),
  capacity_(copy.capacity_)
{
  copy.arr_ = nullptr;
  copy.size_ = 0;
}

template< typename T >
bool nikonov::Stack< T >::empty() const noexcept
{
  return size_ == 0;
}
template< typename T >
size_t nikonov::Stack< T >::size() const noexcept
{
  return size_;
}
template< typename T >
T & nikonov::Stack< T >::top() noexcept
{
  assert(size_ != 0);
  return arr_[size - 1];
}
template< typename T >
const T & nikonov::Stack< T >::top() const noexcept
{
  assert(size_ != 0);
  return arr_[size - 1];
}
template< typename T >
void nikonov::Stack< T >::push(const T & val)
{
  push(std::move(val));
}
template< typename T >
void nikonov::Stack< T >::push(T && val)
{
  if (size_ == capacity_)
  {
    reallocate(capacity_ * reallocScale);
  }
  arr_[size_] = new T(val);
  ++size_;
}
template< typename T >
template< typename... Args >
void nikonov::Stack< T >::emplace(Args &&... args)
{
  if (size_ == capacity_)
  {
    reallocate(capacity_ * reallocScale);
  }
  T * data = new T(std::forward< Args >(args)...);
  arr_[size_] = data;
  ++size_;
}
template< typename T >
void nikonov::Stack< T >::pop() noexcept
{
  assert(size_ != 0);
  delete *(arr_[size - 1]);
  --size_;
}
#endif
