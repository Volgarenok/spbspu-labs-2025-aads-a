#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <algorithm>
#include <stdexcept>

namespace abramov
{
  template< class T >
  struct Stack
  {
    Stack();
    Stack(const Stack< T > &stack);
    Stack(Stack< T > &&stack) noexcept;
    ~Stack();
    Stack< T > &operator=(const Stack< T > &stack);
    Stack< T > &operator=(Stack< T > &&stack);
    void push(T rhs);
    T &top();
    const T &top() const;
    void pop();
    size_t size() const;
    bool empty() const;
    void swap(Stack< T > &stack) noexcept;
  private:
    T *data_;
    size_t size_;
    size_t capacity_;
  };

  namespace
  {
    template< class T >
    T* expandArray(T *data, size_t &capacity)
    {
      constexpr size_t k = 100;
      T *array = new T[capacity + k];
      for (size_t i = 0; i < capacity; ++i)
      {
        array[i + k] = data[i];
      }
      capacity += k;
      return array;
    }
  }

  template< class T >
  Stack< T >::Stack():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  template< class T >
  Stack< T >::Stack(const Stack< T > &stack):
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
    T *data = new T[stack.capacity_];
    try
    {
      for (size_t i = stack.capacity_ - 1; i >= stack.capacity_ - stack.size_; --i)
      {
        data[i] = stack.data_[i];
      }
    }
    catch (const std::exception &)
    {
      delete[] data;
      throw;
    }
    data_ = data;
    capacity_ = stack.capacity_;
    size_ = stack.size_;
  }

  template< class T >
  Stack< T > &Stack< T >::operator=(const Stack< T > &stack)
  {
    Stack< T > copy(stack);
    swap(copy);
    return *this;
  }

  template< class T >
  Stack< T >::Stack(Stack< T > &&stack) noexcept:
    data_(stack.data_),
    size_(stack.size_),
    capacity_(stack.capacity_)
  {
    stack.data_ = nullptr;
  }

  template< class T >
  Stack< T > &Stack< T >::operator=(Stack< T > &&stack)
  {
    Stack< T > copy(stack);
    delete[] data_;
    swap(copy);
    return *this;
  }

  template< class T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }

  template< class T >
  void Stack< T >::push(T rhs)
  {
    if (size_ == capacity_)
    {
      T *new_data = expandArray(data_, capacity_);
      delete data_;
      data_ = new_data;
    }
    data_[capacity_ - size_ - 1] = rhs;
    ++size_;
  }

  template< class T >
  const T &Stack < T >::top() const
  {
    return data_[capacity_ - size_];
  }

  template< class T >
  T &Stack< T >::top()
  {
    return const_cast< T& >(const_cast< const Stack< T >* >(this)->top());
  }

  template< class T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty\n");
    }
    --size_;
  }

  template< class T >
  size_t Stack< T >::size() const
  {
    return size_;
  }

  template< class T >
  bool Stack< T >::empty() const
  {
    return size_ == 0;
  }

  template< class T >
  void Stack< T >::swap(Stack< T > &rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
  }
}
#endif
