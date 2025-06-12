#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <iostream>
#include <stdexcept>


namespace averenkov
{
  template< class T >
  class Array
  {
  public:
    Array();
    Array(const Array &rhs);
    Array(Array &&rhs) noexcept;
    Array &operator=(const Array &rhs);
    ~Array();

    bool empty() const noexcept;
    size_t size() const noexcept;

    const T& front() const noexcept;
    T& front() noexcept;
    const T& back() const noexcept;
    T& back() noexcept;

    void push_back(T rhs);
    void pop_front();
    void pop_back() noexcept;

  private:
    T* data_;
    size_t last_;
    size_t capacity_;
    size_t first_;
    void resize();

  };

  template< class T >
  Array< T >::Array():
    data_(new T[1]),
    last_(0),
    capacity_(1),
    first_(0)
  {}

  template< class T >
  Array< T >::Array(const Array& rhs):
    data_(nullptr),
    last_(rhs.last_),
    capacity_(rhs.capacity_),
    first_(rhs.first_)
  {
    T* temp = nullptr;
    try
    {
      temp = new T[rhs.capacity_];
      for (size_t i = 0; i < last_; ++i)
      {
        temp[i] = rhs.data_[i];
      }
    }
    catch (...)
    {
      delete[] temp;
      temp = nullptr;
    }
    std::swap(temp, data_);
  }


  template< class T >
  Array< T >::Array(Array&& rhs) noexcept:
    data_(rhs.data_),
    last_(rhs.last_),
    capacity_(rhs.capacity_),
    first_(rhs.first_)
  {
    rhs.data_ = nullptr;
    rhs.last_ = 0;
    rhs.capacity_ = 0;
    rhs.first_ = 0;
  }

  template< class T >
  Array< T >& Array< T >::operator=(const Array& rhs)
  {
    if (this != std::addressof(rhs))
    {
      Array< T > temp(rhs);
      std::swap(data_, temp.data_);
      last_ = rhs.last_;
      capacity_ = rhs.capacity_;
      first_ = rhs.first_;
    }
    return *this;
  }

  template< class T >
  Array< T >::~Array()
  {
    delete[] data_;
  }

  template< class T >
  bool Array< T >::empty() const noexcept
  {
    return last_ == first_;
  }

  template< class T >
  size_t Array< T >::size() const noexcept
  {
    return last_ - first_;
  }

  template< class T >
  const T& Array< T >::front() const noexcept
  {
    return data_[first_];
  }

  template< class T >
  T& Array< T >::front() noexcept
  {
    return data_[first_];
  }

  template< class T >
  const T& Array< T >::back() const noexcept
  {
    return data_[last_ - 1];
  }

  template< class T >
  T& Array< T >::back() noexcept
  {
    return data_[last_ - 1];
  }

  template< class T >
  void Array< T >::push_back(T rhs)
  {
    if (last_ == capacity_)
    {
      resize();
    }
    data_[last_++] = rhs;
  }

  template< class T >
  void Array< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("empty");
    }
    ++first_;
  }

  template< class T >
  void Array< T >::pop_back() noexcept
  {
    if (!empty())
    {
      --last_;
    }
  }

  template< class T >
  void Array< T >::resize()
  {
    size_t new_capacity = capacity_ * 2;
    T* new_data = nullptr;
    try
    {
      new_data = new T[new_capacity];
      for (size_t i = 0; i < last_; ++i)
      {
        new_data[i] = data_[i];
      }
    }
    catch (...)
    {
      delete[] new_data;
      return;
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }
}


#endif

