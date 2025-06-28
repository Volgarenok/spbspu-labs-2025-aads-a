#ifndef STACK_HPP
#define STACK_HPP
#include <cassert>
#include <cstddef>
#include <stdexcept>
namespace evstyunichev
{
  namespace
  {
    template< class T >
    void copy_arr(const T *prev, T *cur, size_t sz);

    template< class T >
    void copy_arr(const T *prev, T *cur, size_t sz)
    {
      for (size_t i = 0; i < sz; i++)
      {
        cur[i] = prev[i];
      }
    }
  }

  template< class T >
  class Stack
  {
      template< class U >
      friend void swap(Stack< U > &a, Stack< U > &b);
    public:
      Stack();
      Stack(const Stack< T > &lhs);
      Stack(Stack< T > &&rhs) noexcept;
      ~Stack();
      Stack< T > & operator=(const Stack< T > &lhs);
      Stack< T > & operator=(Stack< T > &&rhs);
      T & top() const;
      bool empty() const;
      void push(const T &value);
      void push(T &&value);
      void pop();
      size_t size();

    private:
      T *data_;
      size_t size_;
      size_t capacity_;
      void clear();
      void resize(size_t);
      void release();
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
    assert(!empty());
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
    T *temp = nullptr;
    try
    {
      T *temp = new T[new_sz];
      copy_arr(data_, temp, size_);
      delete[] data_;
      data_ = temp;
      capacity_ = new_sz;
    }
    catch (const std::exception &e)
    {
      delete[] temp;
      throw;
    }
  }

  template< class T >
  void Stack< T >::push(const T &value)
  {
    if (size_ == capacity_)
    {
      resize(capacity_ * 2);
    }
    data_[size_++] = value;
  }

  template< class T >
  void Stack< T >::push(T &&value)
  {
    if (size_ == capacity_)
    {
      resize(capacity_ * 2);
    }
    data_[size_++] = value;
  }

  template< class T >
  Stack< T >::Stack():
    data_(new T[4]),
    size_(0),
    capacity_(4)
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

  template< class T >
  void Stack< T >::release()
  {
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }

  template< class T >
  Stack< T >::Stack(const Stack< T > &lhs):
    data_(nullptr),
    size_(lhs.size_),
    capacity_(lhs.capacity_)
  {
    T *temp = nullptr;
    try
    {
      T *temp = new T[lhs.capacity_];
      copy_arr(lhs.data_, temp, lhs.size_);
      data_ = temp;
    }
    catch (const std::exception &e)
    {
      delete[] temp;
      throw;
    }
  }

  template< class T >
  Stack< T >::Stack(Stack< T > &&rhs) noexcept:
    data_(rhs.data_),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {
    rhs.release();
  }

  template< class T >
  Stack< T > & Stack< T >::operator=(const Stack < T > &lhs)
  {
    clear();
    T *temp = nullptr;
    try
    {
      temp = new T[lhs.capacity_];
      copy_arr(lhs.data_, temp, lhs.size_);
      size_ = lhs.size_;
      capacity_ = lhs.capacity_;
      data_ = temp;
    }
    catch (const std::exception &e)
    {
      delete[] temp;
      throw;
    }
    return *this;
  }

  template< class T >
  Stack< T > & Stack< T >::operator=(Stack < T > &&rhs)
  {
    clear();
    release();
    swap(*this, rhs);
    return *this;
  }

  template< class T, class U >
  void swap(Stack< U > &a, Stack< U > &b)
  {
    swap(a.data_, b.data_);
    swap(a.size_, b.size_);
    swap(a.capacity_, b.capacity_);
  }

}
#endif
