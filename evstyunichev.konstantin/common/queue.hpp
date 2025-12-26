#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cassert>
#include <cstddef>
#include <stdexcept>
namespace evstyunichev
{
  namespace
  {
    template< class T >
    void copy_arr(const T *prev, T *cur, size_t begin, size_t sz);

    template< class T >
    void copy_arr(const T *prev, T *cur, size_t begin, size_t sz)
    {
      for (size_t i = 0; i < sz; i++)
      {
        cur[i] = prev[(i + begin) % sz];
      }
    }
  }

  template< class T >
  class Queue
  {
      template< class U >
      friend void swap(Queue< U > &a, Queue< U > &b);
    public:
      Queue();
      Queue(const Queue< T > &lhs);
      Queue(Queue< T > &&rhs) noexcept;
      ~Queue();
      Queue< T > & operator=(const Queue< T > &lhs);
      Queue< T > & operator=(Queue< T > &&rhs);
      T & front() const;
      T & back() const;
      bool empty() const;
      void push(const T &value);
      void push(T &&value);
      void pop();
      size_t size();

    private:
      T *data_;
      size_t size_;
      size_t begin_;
      size_t capacity_;
      void clear();
      void resize(size_t new_sz);
      void release();
  };

  template< class T >
  bool Queue< T >::empty() const
  {
    return !size_;
  }

  template< class T >
  T & Queue< T >::back() const
  {
    assert(!empty());
    return data_[(begin_ + size_ - 1) % capacity_];
  }

  template< class T >
  T & Queue< T >::front() const
  {
    assert(!empty());
    return data_[begin_];
  }

  template< class T >
  void Queue< T >::pop()
  {
    assert(!empty());
    --size_;
    begin_ = (begin_ + 1) % capacity_;
  }

  template< class T >
  void Queue< T >::clear()
  {
    delete[] data_;
    release();
  }

  template< class T >
  void Queue< T >::resize(size_t new_sz)
  {
    T *temp = nullptr;
    try
    {
      T *temp = new T[new_sz];
      copy_arr(data_, temp, begin_, capacity_);
      delete[] data_;
      data_ = temp;
      begin_ = 0;
      capacity_ = new_sz;
    }
    catch (const std::exception &e)
    {
      delete[] temp;
      throw;
    }
  }

  template< class T >
  void Queue< T >::push(const T &value)
  {
    if (size_ == capacity_)
    {
      resize(capacity_ * 2);
    }
    data_[(begin_ + size_++) % capacity_] = value;
  }

  template< class T >
  void Queue< T >::push(T &&value)
  {
    if (size_ == capacity_)
    {
      resize(capacity_ * 2);
    }
    data_[(begin_ + size_++) % capacity_] = value;
  }

  template< class T >
  Queue< T >::Queue():
    data_(new T[4]),
    size_(0),
    begin_(0),
    capacity_(4)
  {
  }

  template< class T >
  Queue< T >::~Queue()
  {
    clear();
  }

  template< class T >
  size_t Queue< T >::size()
  {
    return size_;
  }

  template< class T >
  void Queue< T >::release()
  {
    data_ = nullptr;
    size_ = 0;
    begin_ = 0;
    capacity_ = 0;
  }

  template< class T >
  Queue< T >::Queue(const Queue< T > &lhs):
    data_(nullptr),
    size_(lhs.size_),
    begin_(0),
    capacity_(lhs.capacity_)
  {
    T *temp = nullptr;
    try
    {
      T *temp = new T[lhs.capacity_];
      copy_arr(lhs.data_, temp, lhs.begin_, lhs.capacity_);
      data_ = temp;
    }
    catch (const std::exception &e)
    {
      delete[] temp;
      throw;
    }
  }

  template< class T >
  Queue< T >::Queue(Queue< T > &&rhs) noexcept:
    data_(rhs.data_),
    size_(rhs.size_),
    begin_(rhs.begin_),
    capacity_(rhs.capacity_)
  {
    rhs.release();
  }

  template< class T >
  Queue< T > & Queue< T >::operator=(const Queue < T > &lhs)
  {
    clear();
    T *temp = nullptr;
    try
    {
      temp = new T[lhs.capacity_];
      copy_arr(lhs.data_, temp, lhs.begin_, lhs.capacity_);
      size_ = lhs.size_;
      capacity_ = lhs.capacity_;
      begin_ = 0;
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
  Queue< T > & Queue< T >::operator=(Queue < T > &&rhs)
  {
    clear();
    release();
    swap(*this, rhs);
    return *this;
  }

  template< class T, class U >
  void swap(Queue< U > &a, Queue< U > &b)
  {
    swap(a.data_, b.data_);
    swap(a.size_, b.size_);
    swap(a.begin_, b.begin_);
    swap(a.capacity_, b.capacity_);
  }

}
#endif
