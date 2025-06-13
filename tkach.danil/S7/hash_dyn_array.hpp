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
  class HashDynAry
  {
  public:
    HashDynAry();
    HashDynAry(const HashDynAry& other);
    HashDynAry(HashDynAry&& other) noexcept;
    HashDynAry(size_t capacity);
    ~HashDynAry();
    HashDynAry< T >& operator=(HashDynAry< T >&&) noexcept;
    HashDynAry< T >& operator=(const HashDynAry< T >&);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(HashDynAry& other) noexcept;
  private:
    size_t capacity_;
    T* data_;
    void clear();
  };

  template< typename T >
  HashDynAry< T >::HashDynAry():
    capacity_(5),
    data_(new T[5])
  {}

  template< typename T >
  HashDynAry< T >::HashDynAry(size_t capacity):
    capacity_(capacity),
    data_(new T[capacity])
  {}

  template< typename T >
  T& HashDynAry< T >::operator[](size_t index)
  {
    return const_cast< T& >(static_cast< const HashDynAry* >(this)->operator[](index));
  }

  template< typename T >
  const T& HashDynAry< T >::operator[](size_t index) const
  {
    return data_[index];
  }

  template< typename T >
  void HashDynAry< T >::clear()
  {
    if (data_ == nullptr)
    {
      return;
    }
    delete[] data_;
  }

  template< typename T >
  HashDynAry< T >::HashDynAry(const HashDynAry& other):
    capacity_(other.capacity_),
    data_(getDataFromOther(other.data_, other.capacity_))
  {}

  template< typename T >
  HashDynAry< T >::HashDynAry(HashDynAry&& other) noexcept:
    capacity_(other.capacity_),
    data_(other.data_)
  {
    other.capacity_ = 0;
    other.data_ = nullptr;
  }

  template< typename T >
  HashDynAry< T >::~HashDynAry()
  {
    clear();
  }

  template< typename T >
  HashDynAry< T >& HashDynAry< T >::operator=(HashDynAry< T >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    HashDynAry< T > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< typename T >
  HashDynAry< T >& HashDynAry< T >::operator=(const HashDynAry< T >& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    HashDynAry< T > temp(other);
    swap(temp);
    return *this;
  }

  template< typename T >
  bool HashDynAry< T >::empty() const noexcept
  {
    return capacity_ == 0;
  }

  template< typename T >
  size_t HashDynAry< T >::size() const noexcept
  {
    return capacity_;
  }

  template< typename T >
  void HashDynAry< T >::swap(HashDynAry< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
  }
}
#endif
