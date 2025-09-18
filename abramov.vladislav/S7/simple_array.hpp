#ifndef SIMPLE_ARRAY_HPP
#define SIMPLE_ARRAY_HPP
#include <cstddef>
#include <algorithm>

namespace abramov
{
  template< class T >
  struct SimpleArray
  {
    SimpleArray();
    SimpleArray(const SimpleArray< T > &other);
    ~SimpleArray();
    SimpleArray< T > &operator=(const SimpleArray< T > &other);
    T &operator[](size_t id) noexcept;
    const T &operator[](size_t id) const noexcept;
    void pushBack(const T &val);
    size_t size() const noexcept;
    bool empty() const noexcept;
  private:
    T *data_;
    size_t capacity_;
    size_t size_;

    void resize(size_t k);
    void swap(SimpleArray< T > &rhs) noexcept;
  };
}

template< class T >
abramov::SimpleArray< T >::SimpleArray():
  data_(nullptr),
  capacity_(0),
  size_(0)
{}

template< class T >
abramov::SimpleArray< T >::SimpleArray(const SimpleArray< T > &other):
  data_(new T[other.capacity_]),
  capacity_(other.capacity_),
  size_(other.size_)
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = other.data_[i];
  }
}

template< class T >
abramov::SimpleArray< T >::~SimpleArray()
{
  delete[] data_;
}

template< class T >
void abramov::SimpleArray< T >::swap(SimpleArray< T > &other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
}

template< class T >
abramov::SimpleArray< T > &abramov::SimpleArray< T >::operator=(const SimpleArray< T > &other)
{
  SimpleArray< T > temp(other);
  swap(temp);
  return *this;
}

template< class T >
const T &abramov::SimpleArray< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T &abramov::SimpleArray< T >::operator[](size_t id) noexcept
{
  return const_cast< T& >(static_cast< const SimpleArray& >(*this)[id]);
}

template< class T >
void abramov::SimpleArray< T >::pushBack(const T &val)
{
  if (size_ >= capacity_)
  {
    if (capacity_)
    {
      capacity_ *= 2;
    }
    else
    {
      capacity_ = 10;
    }
    resize(capacity_);
  }
  data_[size_++] = val;
}

template< class T >
void abramov::SimpleArray< T >::resize(size_t k)
{
  T *new_data = new T[k];
  for (size_t i = 0; i < size_; ++i)
  {
    new_data[i] = data_[i];
  }
  delete[] data_;
  data_ = new_data;
}

template< class T >
size_t abramov::SimpleArray< T >::size() const noexcept
{
  return size_;
}

template< class T >
bool abramov::SimpleArray< T >::empty() const noexcept
{
  return size_ == 0;
}
#endif
