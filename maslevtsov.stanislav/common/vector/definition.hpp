#ifndef VECTOR_DEFINITION_HPP
#define VECTOR_DEFINITION_HPP

#include <utility>
#include "declaration.hpp"
#include "vector_iterator.hpp"

template< class T >
maslevtsov::Vector< T >::Vector():
  capacity_(16),
  size_(0),
  data_(new T[16]{})
{}

template< class T >
maslevtsov::Vector< T >::Vector(const Vector& rhs):
  capacity_(rhs.capacity_),
  size_(rhs.size_),
  data_(new T[rhs.capacity_]{})
{
  try {
    for (size_t i = 0; i < rhs.size_; ++i) {
      data_[i] = rhs.data_[i];
    }
  } catch (...) {
    delete[] data_;
    throw;
  }
}

template< class T >
maslevtsov::Vector< T >::Vector(Vector&& rhs) noexcept:
  capacity_(std::exchange(rhs.capacity_, 0)),
  size_(std::exchange(rhs.size_, 0)),
  data_(std::exchange(rhs.data_, nullptr))
{}

template< class T >
maslevtsov::Vector< T >::Vector(size_t size):
  capacity_(size),
  size_(size),
  data_(new T[size]{})
{}

template< class T >
maslevtsov::Vector< T >::~Vector()
{
  delete[] data_;
}

template< class T >
typename maslevtsov::Vector< T >::Vector& maslevtsov::Vector< T >::operator=(const Vector& rhs)
{
  if (this != &rhs) {
    Vector< T > copied_rhs(rhs);
    swap(copied_rhs);
  }
  return *this;
}

template< class T >
typename maslevtsov::Vector< T >::Vector& maslevtsov::Vector< T >::operator=(Vector&& rhs) noexcept
{
  if (this != &rhs) {
    Vector< T > moved_rhs(std::move(rhs));
    swap(moved_rhs);
  }
  return *this;
}

template< class T >
T& maslevtsov::Vector< T >::operator[](size_t pos) noexcept
{
  return data_[pos];
}

template< class T >
const T& maslevtsov::Vector< T >::operator[](size_t pos) const noexcept
{
  return data_[pos];
}

template< class T >
T& maslevtsov::Vector< T >::front()
{
  return data_[0];
}

template< class T >
const T& maslevtsov::Vector< T >::front() const
{
  return data_[0];
}

template< class T >
T& maslevtsov::Vector< T >::back()
{
  return data_[size_ - 1];
}

template< class T >
const T& maslevtsov::Vector< T >::back() const
{
  return data_[size_ - 1];
}

template< class T >
typename maslevtsov::Vector< T >::iterator maslevtsov::Vector< T >::begin()
{
  return iterator(data_);
}

template< class T >
typename maslevtsov::Vector< T >::const_iterator maslevtsov::Vector< T >::begin() const
{
  return const_iterator(data_);
}

template< class T >
typename maslevtsov::Vector< T >::const_iterator maslevtsov::Vector< T >::cbegin() const noexcept
{
  return const_iterator(data_);
}

template< class T >
typename maslevtsov::Vector< T >::iterator maslevtsov::Vector< T >::end()
{
  return iterator(data_ + size_);
}

template< class T >
typename maslevtsov::Vector< T >::const_iterator maslevtsov::Vector< T >::end() const
{
  return const_iterator(data_ + size_);
}

template< class T >
typename maslevtsov::Vector< T >::const_iterator maslevtsov::Vector< T >::cend() const noexcept
{
  return const_iterator(data_ + size_);
}

template< class T >
bool maslevtsov::Vector< T >::empty() const noexcept
{
  return size_ == 0;
}

template< class T >
size_t maslevtsov::Vector< T >::size() const noexcept
{
  return size_;
}

template< class T >
void maslevtsov::Vector< T >::clear() noexcept
{
  while (!empty()) {
    pop_back();
  }
}

template< class T >
typename maslevtsov::Vector< T >::iterator maslevtsov::Vector< T >::erase(iterator pos)
{
  return erase(const_iterator(pos.ptr_));
}

template< class T >
typename maslevtsov::Vector< T >::iterator maslevtsov::Vector< T >::erase(const_iterator pos)
{
  if (pos == cend()) {
    return end();
  }
  size_t index = 0;
  for (auto i = cbegin(); i != pos; ++i) {
    ++index;
  }
  if (index >= size_) {
    throw std::out_of_range("iterator out of range");
  }
  for (size_t i = index; i < size_ - 1; ++i) {
    data_[i] = std::move(data_[i + 1]);
  }
  --size_;
  iterator it = begin();
  for (size_t i = 0; i < index; ++i) {
    ++it;
  }
  return it;
}

template< class T >
void maslevtsov::Vector< T >::push_back(const T& value)
{
  if (size_ >= capacity_) {
    expand(capacity_ * 2);
  }
  data_[size_++] = value;
}

template< class T >
void maslevtsov::Vector< T >::push_back(T&& value)
{
  if (size_ >= capacity_) {
    expand(capacity_ * 2);
  }
  data_[size_++] = std::move(value);
}

template< class T >
void maslevtsov::Vector< T >::pop_back() noexcept
{
  if (empty()) {
    return;
  }
  --size_;
}

template< class T >
void maslevtsov::Vector< T >::expand(size_t count)
{
  T* new_data = new T[count];
  try {
    for (size_t i = 0; i < size_; i++) {
      new_data[i] = data_[i];
    }
  } catch (...) {
    delete[] new_data;
    throw;
  }
  capacity_ = count;
  delete[] data_;
  data_ = new_data;
}

template< class T >
void maslevtsov::Vector< T >::swap(Vector& other) noexcept
{
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(data_, other.data_);
}

#endif
