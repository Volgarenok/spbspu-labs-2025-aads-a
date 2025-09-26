#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>

namespace kushekbaev
{
  template< typename T >
  class Vector
  {
    public:
      Vector() noexcept;
      Vector(const Vector& other);
      Vector(Vector&& other) noexcept;
      ~Vector() noexcept;

      Vector& operator=(const Vector& other);
      Vector& operator=(Vector&& other) noexcept;

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      T& operator[](size_t index) noexcept;
      const T& operator[](size_t index) const noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      void pushBack(const T& value);
      void popFront();
      void popBack() noexcept;

    private:
      T* data_;
      size_t size_;
      size_t capacity_;
      void extendVector();
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
  Vector< T >::Vector() noexcept:
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
  Vector< T >::Vector(Vector&& other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template< typename T >
  Vector< T >::~Vector() noexcept
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
  Vector< T >& Vector< T >::operator=(Vector&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      std::swap(data_, other.data_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
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
  T& Vector< T >::operator[](size_t index) noexcept
  {
    return data_[index];
  }

  template< typename T >
  const T& Vector< T >::operator[](size_t index) const noexcept
  {
    return data_[index];
  }

  template< typename T >
  const T& Vector< T >::back() const noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  void Vector< T >::pushBack(const T& value)
  {
    if (size_ == capacity_)
    {
      extendVector();
    }
    data_[size_++] = value;
  }

  template< typename T >
  void Vector< T >::popFront()
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
  void Vector< T >::popBack() noexcept
  {
    if (!empty())
    {
      --size_;
    }
  }

  template< typename T >
  void Vector< T >::extendVector()
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
