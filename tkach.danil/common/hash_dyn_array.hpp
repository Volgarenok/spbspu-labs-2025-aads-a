#ifndef HASH_DYN_ARRAY_HPP
#define HASH_DYN_ARRAY_HPP

#include <utility>
#include <cstddef>

namespace
{
  template< typename T >
  T* getDataFromOther(const T* data, const size_t capacity)
  {
    T* new_data = new T[capacity];
    try
    {
      for (size_t i = 0; i < capacity; ++i)
      {
        new_data[i] = data[i];
      }
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
    return new_data;
  }
}

namespace tkach
{
  template< typename T >
  class HashDynArray
  {
  public:
    HashDynArray();
    HashDynArray(const HashDynArray& other);
    HashDynArray(HashDynArray&& other) noexcept;
    HashDynArray(size_t capacity);
    ~HashDynArray();
    HashDynArray< T >& operator=(HashDynArray< T >&&) noexcept;
    HashDynArray< T >& operator=(const HashDynArray< T >&);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(HashDynArray& other) noexcept;
  private:
    size_t capacity_;
    T* data_;
    void clear();
  };

  template< typename T >
  HashDynArray< T >::HashDynArray():
    capacity_(5),
    data_(new T[5])
  {}

  template< typename T >
  HashDynArray< T >::HashDynArray(size_t capacity):
    capacity_(capacity),
    data_(new T[capacity])
  {}

  template< typename T >
  T& HashDynArray< T >::operator[](size_t index)
  {
    return const_cast< T& >(static_cast< const HashDynArray* >(this)->operator[](index));
  }

  template< typename T >
  const T& HashDynArray< T >::operator[](size_t index) const
  {
    return data_[index];
  }

  template< typename T >
  void HashDynArray< T >::clear()
  {
    if (data_ == nullptr)
    {
      return;
    }
    delete[] data_;
  }

  template< typename T >
  HashDynArray< T >::HashDynArray(const HashDynArray& other):
    capacity_(other.capacity_),
    data_(getDataFromOther(other.data_, other.capacity_))
  {}

  template< typename T >
  HashDynArray< T >::HashDynArray(HashDynArray&& other) noexcept:
    capacity_(other.capacity_),
    data_(other.data_)
  {
    other.capacity_ = 0;
    other.data_ = nullptr;
  }

  template< typename T >
  HashDynArray< T >::~HashDynArray()
  {
    clear();
  }

  template< typename T >
  HashDynArray< T >& HashDynArray< T >::operator=(HashDynArray< T >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    HashDynArray< T > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< typename T >
  HashDynArray< T >& HashDynArray< T >::operator=(const HashDynArray< T >& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    HashDynArray< T > temp(other);
    swap(temp);
    return *this;
  }

  template< typename T >
  bool HashDynArray< T >::empty() const noexcept
  {
    return capacity_ == 0;
  }

  template< typename T >
  size_t HashDynArray< T >::size() const noexcept
  {
    return capacity_;
  }

  template< typename T >
  void HashDynArray< T >::swap(HashDynArray< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
  }
}
#endif
