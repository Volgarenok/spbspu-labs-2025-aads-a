#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>

namespace kushekbaev
{
  template< typename T >
  class Vector
  {
    public:
      Vector();
      Vector(const Vector& other);
      Vector(Vector&& other);
      ~Vector();

      Vector& operator=(const Vector& other);

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      void push_back(const T& value);
      void pop_front();
      void pop_back();

    private:
      T* data_;
      size_t size_;
      size_t capacity_;
      void extend_vector();
      class RAII
      {
        public:
          explicit RAII(T* p):
            ptr(p)
          {}

          ~RAII()
          {
            delete[] ptr;
          }

          RAII(RAII&& other) noexcept:
            ptr(other.ptr)
          {
            other.ptr = nullptr;
          }

          T* ptr;
      };
  };

  template< typename T >
  Vector< T >::Vector():
    data_(new T[1]),
    size_(0),
    capacity_(1)
  {}

  template< typename T >
  Vector< T >::Vector(const Vector& other):
    data_(nullptr),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    RAII newHolder(new T[other.capacity_]);
    for (size_t i = 0; i < size_; ++i)
    {
      newHolder.ptr[i] = other.data_[i];
    }
    data_ = newHolder.ptr;
    newHolder.ptr = nullptr;
  }

  template< typename T >
  Vector< T >::Vector(Vector&& other):
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template< typename T >
  Vector< T >::~Vector()
  {
    delete[] data_;
  }

  template< typename T >
  Vector< T >& Vector< T >::operator=(const Vector& other)
  {
    if (this != std::addressof(other))
    {
      RAII newHolder(new T[other.capacity_]);
      for (size_t i = 0; i < other.size_; ++i)
      {
        newHolder.ptr[i] = other.data_[i];
      }
      delete[] data_;
      data_ = newHolder.ptr;
      newHolder.ptr = nullptr;
      capacity_ = other.capacity_;
      size_ = other.size_;
    }
    return *this;
  }

  template< typename T >
  bool Vector< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t Vector< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  T& Vector< T >::front() noexcept
  {
    return data_[0];
  }

  template< typename T >
  T& Vector< T >::back() noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  const T& Vector< T >::front() const noexcept
  {
    return data_[0];
  }

  template< typename T >
  const T& Vector< T >::back() const noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  void Vector< T >::push_back(const T& value)
  {
    if (size_ == capacity_)
    {
      extend_vector();
    }
    data_[size_++] = value;
  }

  template< typename T >
  void Vector< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("The array is empty!");
    }
    for (size_t i = 1; i < size_; ++i)
    {
      data_[i - 1] = std::move(data_[i]);
    }
    --size_;
  }

  template< typename T >
  void Vector< T >::pop_back()
  {
    if (!empty())
    {
      --size_;
    }
  }

  template< typename T >
  void Vector< T >::extend_vector()
  {
    size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    RAII newHolder(new T[newCapacity]);
    for (size_t i = 0; i < size_; ++i)
    {
      newHolder.ptr[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = newHolder.ptr;
    newHolder.ptr = nullptr;
    capacity_ = newCapacity;
  }
}

#endif
