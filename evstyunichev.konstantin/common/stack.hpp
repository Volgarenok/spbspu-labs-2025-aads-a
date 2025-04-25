#ifndef STACK_HPP
#define STACK_HPP
#include <cassert>
#include <cmath>
#include <cstddef>
namespace evstyunichev
{
  template< class T >
  class Stack
  {
    public:
      Stack();
      ~Stack();
      T & top() const;
      bool empty() const;
      void push(const T &value);
      void push(T &&value);
      void pop();
      size_t size();
    private:
      T *data_;
      size_t size_;
      size_t capasity_;
      void clear();
      void resize(size_t);

  };

  template< class T >
  bool Stack< T >::empty() const
  {
    return !size_;
  }

  template< class T >
  T & Stack< T >::top() const
  {
    assert(!empty());
    return data_[size_ - 1];
  }

  template< class T >
  void Stack< T >::pop()
  {
    --size_;
  }

  template< class T >
  void Stack< T >::clear()
  {
    delete[] data_;
  }

  template< class T >
  void Stack< T >::resize(size_t new_sz)
  {
    T *temp = new T[new_sz];
    for (size_t i = 0; i < std::min(size_, new_sz); i++)
    {
      temp[i] = data_[i];
    }
    delete[] data_;
    data_ = temp;
    capasity_ = new_sz;
  }

  template< class T >
  void Stack< T >::push(const T &value)
  {
    if (size_ == capasity_)
    {
      resize(capasity_ * 2);
    }
    data_[size_++] = value;
  }

  template< class T >
  void Stack< T >::push(T &&value)
  {
    if (size_ >= capasity_)
    {
      resize(capasity_ * 2);
    }
    data_[size_++] = value;
  }

  template< class T >
  Stack< T >::Stack():
    data_(new T[4]),
    size_(0),
    capasity_(4)
  {
  }

  template< class T >
  Stack< T >::~Stack()
  {
    clear();
  }

  template< class T >
  size_t Stack< T >::size()
  {
    return size_;
  }

}
#endif
