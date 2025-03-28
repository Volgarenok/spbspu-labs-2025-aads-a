#ifndef DYNAMICARR_HPP
#define DYNAMICARR_HPP

#include <cstddef>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace bocharov
{
  template< typename T >
  class Array
  {
  public:
    Array();
    Array(const Array< T > &);
    Array(Array< T > &&) noexcept;
    Array(size_t capacity);
    ~Array();

    Array< T > & operator=(const Array< T > &);
    Array< T > & operator=(Array< T > &&) noexcept;

    T & back();
    const T & back() const;
    T & front();
    const T & front() const;
    void popBack();
    void popFront();
    void push(T &) noexcept;
    void push(T &&) noexcept;
    void clear() noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

  private:

    T ** data_;
    size_t capacity_;
    size_t size_;
    size_t begin_;

    void reallocate();
    void swap(Array< T > &) noexcept;
  };

  template< typename T >
  void Array< T >::reallocate()
  {
    size_t newCapacity = capacity_ * 2;
    Array< T > newArray(newCapacity);
    for (size_t i = 0; i < size_; ++i)
    {
      newArray.push(*data_[i + begin_]);
    }
    swap(newArray);
  }

  template< typename T >
  void Array< T >::clear() noexcept
  {
    while(!empty())
    {
      popFront();
    }
    size_ = 0;
    begin_ = 0;
  }

  template< typename T >
  void Array< T >::swap(Array< T > & arr) noexcept
  {
    std::swap(data_, arr.data_);
    std::swap(capacity_, arr.capacity_);
    std::swap(size_, arr.size_);
    std::swap(begin_, arr.begin_);
  }

  template< typename T >
  Array< T >::Array():
    data_(nullptr),
    capacity_(5),
    size_(0),
    begin_(0)
  {
    data_ = new T * [capacity_];
  }

  template< typename T >
  Array< T >::Array(const Array< T > & arr):
    data_(new T * [arr.capacity_]),
    capacity_(arr.capacity_),
    size_(arr.size_),
    begin_(arr.begin_)
  {
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i] = new T(*arr.data_[i + begin_]);
      }
    }
    catch (const std::bad_alloc &)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  Array< T >::Array(Array< T > && arr) noexcept:
    data_(std::exchange(arr.data_, nullptr)),
    capacity_(std::exchange(arr.capacity_, 0)),
    size_(std::exchange(arr.size_, 0)),
    begin_(std::exchange(arr.begin_, 0))
  {}

  template< typename T >
  Array< T >::Array(size_t capacity):
    data_(new T * [capacity]),
    capacity_(capacity),
    size_(0),
    begin_(0)
  {}

  template< typename T >
  Array< T >::~Array()
  {
    clear();
    delete[] data_;
    capacity_ = 0;
  }

  template< typename T >
  Array< T > & Array< T >::operator=(const Array< T > & arr)
  {
    Array< T > copy(arr);
    swap(copy);
    return *this;
  }

  template< typename T >
  Array< T > & Array< T >::operator=(Array< T > && arr) noexcept
  {
    Array< T > temp(std::move(arr));
    swap(temp);
    return *this;
  }

  template< typename T >
  T & Array< T >::back()
  {
    return const_cast< T & >(static_cast< const Array< T > & >(*this).back());
  }

  template< typename T >
  const T & Array< T >::back() const
  {
    if (empty())
    {
      throw std::logic_error("Empty for back()");
    }
    return *(data_[begin_ + size_ - 1]);
  }

  template< typename T >
  const T & Array< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("Empty for front()");
    }
    return *(data_[begin_]);
  }

  template< typename T >
  T & Array< T >::front()
  {
    return const_cast< T & >(static_cast< const Array< T > & >(*this).front());
  }

  template< typename T >
  void Array< T >::popBack()
  {
    if (empty())
    {
      throw std::logic_error("Empty for popBack()");
    }
    delete data_[begin_ + size_ - 1];
    --size_;
  }

  template< typename T >
  void Array< T >::popFront()
  {
    if (empty())
    {
      throw std::logic_error("Empty for popFront()");
    }
    delete data_[begin_];
    ++begin_;
    --size_;
  }

  template< typename T >
  void Array< T >::push(T & data) noexcept
  {
    if (size_ >= capacity_)
    {
      reallocate();
    }
    data_[begin_ + size_] = new T(data);
    ++size_;
  }

  template< typename T >
  void Array< T >::push(T && data) noexcept
  {
    if (size_ >= capacity_)
    {
      reallocate();
    }
    data_[begin_ + size_] = new T(std::move(data));
    ++size_;
  }

  template< typename T >
  size_t Array< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  bool Array< T >::empty() const noexcept
  {
    return size_ == 0;
  }
}

#endif
