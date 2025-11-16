#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <iostream>
#include <stdexcept>
#include <iteratorArray.hpp>

namespace averenkov
{
  template< class T, bool IsConst >
  class ArrayIterator;

  template< class T >
  class Array
  {
  public:
    using Iterator = ArrayIterator< T, false >;
    using ConstIterator = ArrayIterator< T, true >;

    Array();
    Array(const Array &rhs);
    Array(Array &&rhs) noexcept;
    Array &operator=(const Array &rhs) noexcept;
    Array &operator=(Array &&rhs) noexcept;
    explicit Array(size_t size);
    ~Array();

    bool empty() const noexcept;
    size_t size() const noexcept;

    void swap(Array& rhs) noexcept;
    void clear();

    const T& front() const noexcept;
    T& front() noexcept;
    const T& back() const noexcept;
    T& back() noexcept;

    void push_back(T rhs);
    void pop_front();
    void pop_back() noexcept;

    const T& operator[](size_t index) const;
    T& operator[](size_t index);

    T* get_data();
    const T* get_data() const;

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    void resize(size_t capac);
    void resize();

  private:
    T* data_;
    size_t last_;
    size_t capacity_;
    size_t first_;
    Array< T > copy(const Array& other, size_t capacity);

    T* copy_data(const Array& other, size_t capacity);

  };

  template< class T >
  T* Array< T >::get_data()
  {
    return data_;
  }

  template< class T >
  const T* Array< T >::get_data() const
  {
    return data_;
  }

  template< class T >
  Array< T >::Array():
    data_(new T[1]),
    last_(0),
    capacity_(1),
    first_(0)
  {}

  template< typename T >
  Array< T >::Array(size_t size):
    data_(new T[size]()),
    last_(size),
    capacity_(size),
    first_(0)
  {
  }

  template< class T >
  Array< T >::Array(const Array& rhs):
    data_(copy_data(rhs, rhs.capacity_)),
    last_(rhs.last_),
    capacity_(rhs.capacity_),
    first_(rhs.first_)
  {}


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
  Array< T >& Array< T >::operator=(const Array& other) noexcept
  {
    Array< T > temp(other);
    swap(temp);
    return *this;
  }

  template< class T >
  Array< T >& Array< T >::operator=(Array&& other) noexcept
  {
    Array< T > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< class T >
  Array< T >::~Array()
  {
    delete[] data_;
  }

  template< class T >
  void Array< T >::clear()
  {
    delete[] data_;
    data_ = new T[1];
    last_ = 0;
    first_ = 0;
    capacity_ = 1;
  }

  template< class T >
  void Array< T >::swap(Array< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(last_, other.last_);
    std::swap(capacity_, other.capacity_);
    std::swap(first_, other.first_);
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
  const T& Array< T >::operator[](size_t index) const
  {
    if (index >= size())
    {
      throw std::out_of_range("Index out of range");
    }
    return data_[first_ + index];
  }

  template< class T >
  T& Array< T >::operator[](size_t index)
  {
    if (index >= size())
    {
      throw std::out_of_range("Index out of range");
    }
    return data_[first_ + index];
  }

  template< class T >
  void Array< T >::resize(size_t capac)
  {
    auto arr = copy(*this, capac * 2);
    swap(arr);
  }

  template< class T >
  void Array< T >::resize()
  {
    auto arr = copy(*this, capacity_ * 2);
    swap(arr);
  }

  template< class T >
  Array< T > Array< T >::copy(const Array& other, size_t capacity)
  {
    Array< T > new_array;
    T* new_data = nullptr;
    try
    {
      new_data = new T[capacity];
      for (size_t i = 0; i < other.last_; ++i)
      {
        new_data[i] = other.data_[i];
      }
      delete[] new_array.data_;
      new_array.data_ = new_data;
      new_array.capacity_ = capacity;
      new_array.last_ = other.last_;
      new_array.first_ = other.first_;
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
    return new_array;
  }

  template< class T >
  T* Array< T >::copy_data(const Array& other, size_t capacity)
  {
    T* new_data = nullptr;
    try
    {
      new_data = new T[capacity];
      for (size_t i = 0; i < other.last_; ++i)
      {
        new_data[i] = other.data_[i];
      }
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
    return new_data;
  }

  template< class T >
  typename Array< T >::Iterator Array< T >::begin() noexcept
  {
    return Iterator(data_ + first_);
  }

  template< class T >
  typename Array< T >::Iterator Array< T >::end() noexcept
  {
    return Iterator(data_ + last_);
  }

  template< class T >
  typename Array< T >::ConstIterator Array< T >::begin() const noexcept
  {
    return ConstIterator(data_ + first_);
  }

  template< class T >
  typename Array< T >::ConstIterator Array< T >::end() const noexcept
  {
    return ConstIterator(data_ + last_);
  }

  template< class T >
  typename Array< T >::ConstIterator Array< T >::cbegin() const noexcept
  {
    return ConstIterator(data_ + first_);
  }

  template< class T >
  typename Array< T >::ConstIterator Array< T >::cend() const noexcept
  {
    return ConstIterator(data_ + last_);
  }

}

#endif

