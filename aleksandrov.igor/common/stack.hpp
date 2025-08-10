#ifndef STACK_HPP
#define STACK_HPP

#include <utility>
#include <cstddef>
#include <stdexcept>
#include <cassert>

namespace aleksandrov
{
  constexpr size_t minStackCapacity = 64;

  template< class T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack&);
    Stack(Stack&&) noexcept;
    ~Stack() noexcept;

    Stack& operator=(const Stack&);
    Stack& operator=(Stack&&) noexcept;

    T& top();
    const T& top() const;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    void clear() noexcept;
    void push(const T&);
    void push(T&&);
    template< class... Args >
    void emplace(Args&&...);
    void pop();
    void swap(Stack&) noexcept;

  private:
    T* data_;
    T* last_;
    size_t size_;
    size_t capacity_;

    T* copyData(const Stack&);
    void resize();
  };

  template< class T >
  Stack< T >::Stack():
    data_(static_cast< T* >(operator new(minStackCapacity * sizeof(T)))),
    last_(nullptr),
    size_(0),
    capacity_(minStackCapacity)
  {}

  template< class T >
  Stack< T >::Stack(const Stack& rhs):
    data_(copyData(rhs)),
    last_(data_ + rhs.size_ - 1),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {}

  template< class T >
  Stack< T >::Stack(Stack&& rhs) noexcept:
    data_(std::exchange(rhs.data_, nullptr)),
    last_(std::exchange(rhs.last_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0))
  {}

  template< class T >
  Stack< T >::~Stack() noexcept
  {
    clear();
    operator delete(data_);
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(const Stack& rhs)
  {
    Stack newStack(rhs);
    swap(newStack);
    return *this;
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(Stack&& rhs) noexcept
  {
    Stack newStack(std::move(rhs));
    swap(newStack);
    return *this;
  }

  template< class T >
  T& Stack< T >::top()
  {
    assert(!empty());
    return *last_;
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    assert(!empty());
    return *last_;
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return !size_;
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  size_t Stack< T >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class T >
  void Stack< T >::clear() noexcept
  {
    while (!empty())
    {
      pop();
    }
  }

  template< class T >
  void Stack< T >::push(const T& el)
  {
    emplace(el);
  }

  template< class T >
  void Stack< T >::push(T&& el)
  {
    emplace(std::move(el));
  }

  template< class T >
  template< class... Args >
  void Stack< T >::emplace(Args&&... args)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    new (last_ ? ++last_ : last_ = data_) T(std::forward< Args >(args)...);
    ++size_;
  }

  template< class T >
  void Stack< T >::pop()
  {
    assert(!empty());
    (size_-- == 1 ? last_ : last_--)->~T();
  }

  template< class T >
  void Stack< T >::swap(Stack& rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(last_, rhs.last_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
  }

  template< class T >
  T* Stack< T >::copyData(const Stack& rhs)
  {
    T* copy = static_cast< T* >(operator new(rhs.capacity_ * sizeof(T)));
    size_t i = 0;
    try
    {
      for (; i < rhs.size_; ++i)
      {
        new (copy + i) T(rhs.data_[i]);
      }
    }
    catch (const std::bad_alloc&)
    {
      for (size_t j = 0; j < i; ++j)
      {
        (copy + j)->~T();
      }
      operator delete(copy);
      throw;
    }
    return copy;
  }

  template< class T >
  void Stack< T >::resize()
  {
    size_t newCapacity = capacity_ * 2;
    T* newData = static_cast< T* >(operator new(newCapacity * sizeof(T)));
    size_t i = 0;
    try
    {
      for (; i < size_; ++i)
      {
        new (newData + i) T(std::move_if_noexcept(data_[i]));
      }
    }
    catch (const std::bad_alloc&)
    {
      for (size_t j = 0; j < i; ++j)
      {
        newData[j].~T();
      }
      operator delete(newData);
      throw;
    }
    clear();
    operator delete(data_);
    data_ = newData;
    size_ = capacity_;
    capacity_ = newCapacity;
  }
}

#endif

