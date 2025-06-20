#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <algorithm>
#include <cassert>

namespace sveshnikov
{
  template < class T >
  class Stack
  {
  public:
    explicit Stack();
    Stack(const Stack &other);
    Stack(Stack &&other);
    ~Stack();
    Stack< T > &operator=(const Stack &other);
    Stack< T > &operator=(Stack &&other) noexcept;

    T &top() noexcept;
    const T &top() const noexcept;
    size_t getSize() const noexcept;
    bool empty() const noexcept;
    void push(const T &data);
    void push(T &&data);
    void pop();
    void swap(Stack &other) noexcept;

  private:
    size_t capacity_;
    T *data_;
    size_t size_;
    void resize();
    void reset();
    T *formNewData() const;
  };

  template < class T >
  Stack< T >::Stack():
    capacity_(0),
    data_(nullptr),
    size_(0)
  {}

  template < class T >
  Stack< T >::Stack(const Stack &other):
    capacity_(other.capacity_),
    data_(other.formNewData()),
    size_(other.size_)
  {}

  template < class T >
  Stack< T >::Stack(Stack &&other):
    capacity_(other.capacity_),
    data_(other.data_),
    size_(other.size_)
  {
    other.reset();
  }

  template < class T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }

  template < class T >
  Stack< T > &Stack< T >::operator=(const Stack &other)
  {
    if (this != std::addressof(other))
    {
      Stack rhs(other);
      swap(rhs);
    }
    return *this;
  }

  template < class T >
  Stack< T > &Stack< T >::operator=(Stack &&other) noexcept
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
  T &Stack< T >::top() noexcept
  {
    return const_cast< T & >(static_cast< const Stack< T > & >(*this).top());
    ;
  }

  template < class T >
  const T &Stack< T >::top() const noexcept
  {
    assert(!empty());
    return data_[size_ - 1];
  }

  template < class T >
  size_t Stack< T >::getSize() const noexcept
  {
    return size_;
  }

  template < class T >
  bool Stack< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class T >
  void Stack< T >::resize()
  {
    if (size_ == capacity_)
    {
      capacity_ += 100;
      T *new_data = formNewData();
      delete[] data_;
      data_ = new_data;
    }
  }

  template < class T >
  void Stack< T >::push(const T &data)
  {
    resize();
    data_[size_] = data;
    size_++;
  }

  template < class T >
  void Stack< T >::push(T &&data)
  {
    resize();
    data_[size_] = std::move(data);
    size_++;
  }

  template < class T >
  void Stack< T >::pop()
  {
    assert(!empty());
    size_--;
  }

  template < class T >
  void Stack< T >::swap(Stack &other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
  }

  template < class T >
  void Stack< T >::reset()
  {
    capacity_ = 0;
    data_ = nullptr;
    size_ = 0;
  }

  template < class T >
  T *Stack< T >::formNewData() const
  {
    T *new_data = new T[capacity_];
    for (size_t i = 0; i < size_; i++)
    {
      new_data[i] = data_[i];
    }
    return new_data;
  }
}

#endif
