#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <cstddef>
#include <stdexcept>

namespace karnauhova
{
  template < class T >
  class DynamicArray
  {
  public:
    DynamicArray();
    ~DynamicArray();
    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;

    DynamicArray& operator=(const DynamicArray& other);
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void push(const T& data);
    void push(T&& data);
    void pop_back();

    void swap(DynamicArray& other) noexcept;
    void clear();
  private:
    size_t size_;
    size_t capacity_;
    T* data_;

    void resize();
  };

  template < class T >
  DynamicArray< T >::DynamicArray():
    size_(0),
    capacity_(0),
    data_(nullptr)
  {}

  template < class T >
  DynamicArray< T >::~DynamicArray()
  {
    delete[] data_;
  }

  template < class T >
  DynamicArray< T >::DynamicArray(const DynamicArray& other):
    size_(other.size_),
    capacity_(other.capacity_),
    data_(new T[other.capacity_])
  {
    for (size_t i = 0; i < size_; ++i)
    {
      try
      {
        data_[i] = other.data_[i];
      }
      catch (const std::bad_alloc&)
      {
        delete[] data_;
        throw;
      }
    }
  }

  template < class T >
  DynamicArray< T >::DynamicArray(DynamicArray&& other) noexcept:
    size_(other.size_),
    capacity_(other.capacity_),
    data_(other.data_)
  {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
  }

  template < class T >
  DynamicArray< T >& DynamicArray< T >::operator=(const DynamicArray& other)
  {
    if (this != std::addressof(other))
    {
      DynamicArray< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template < class T >
  DynamicArray< T >& DynamicArray< T >::operator=(DynamicArray&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      DynamicArray< T > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template < class T >
  bool DynamicArray< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class T >
  size_t DynamicArray< T >::size() const noexcept
  {
    return size_;
  }

  template < class T >
  T& DynamicArray< T >::front()
  {
    return const_cast< T& >(static_cast< const DynamicArray< T >& >(*this).front());
  }

  template < class T >
  const T& DynamicArray< T >::front() const
  {
    if (empty())
    {
      throw std::out_of_range("Is empty");
    }
    return data_[0];
  }

  template < class T >
  T& DynamicArray< T >::back()
  {
    return const_cast< T& >(static_cast< const DynamicArray< T >& >(*this).back());
  }

  template < class T >
  const T& DynamicArray< T >::back() const
  {
    if (empty())
    {
      throw std::out_of_range("Is empty");
    }
    return data_[size_ - 1];
  }

  template < class T >
  T& DynamicArray< T >::operator[](size_t index)
  {
    return data_[index];
  }

  template < class T >
  const T& DynamicArray< T >::operator[](size_t index) const
  {
    return data_[index];
  }

  template < class T >
  void DynamicArray< T >::push(const T& data)
  {
    if (size_ >= capacity_)
    {
      resize();
    }
    data_[size_] = data;
    size_++;
  }

  template < class T >
  void DynamicArray< T >::push(T&& data)
  {
    if (size_ >= capacity_)
    {
      resize();
    }
    data_[size_] = std::move(data);
    size_++;
  }

  template < class T >
  void DynamicArray< T >::pop_back()
  {
    if (empty())
    {
      throw std::out_of_range("Is empty");
    }
    --size_;
  }

  template < class T >
  void DynamicArray< T >::swap(DynamicArray& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
  }

  template < class T >
  void DynamicArray< T >::resize()
  {
    T* new_data = new T[5 + capacity_];
    try
    {
      for (size_t i = 0; i < size_; i++)
      {
        new_data[i] = data_[i];
      }
    }
    catch (const std::bad_alloc& e)
    {
      delete[] new_data;
      throw;
    }
    delete[] data_;
    data_ = new_data;
    capacity_ += 5;
  }

  template < class T >
  void DynamicArray< T >::clear()
  {
    size_ = 0;
  }
}

#endif
