#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <cstddef>
#include <memory>
#include <utility>

namespace petrov
{
  template< typename T >
  struct DynamicArray
  {
  public:
    using this_t = DynamicArray< T >;
    DynamicArray();
    DynamicArray(const this_t & rhs);
    DynamicArray(this_t && rhs);
    ~DynamicArray();
    this_t & operator=(const this_t & rhs);
    this_t & operator=(this_t && rhs);
    T & operator[](const size_t & pos);
    const T & operator[](const size_t & pos) const;
    template< typename T1 >
    void push_back(T1 && val);
    void pop_back();
    void pop_front();
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(this_t & rhs) noexcept;
  private:
    T * massive_;
    size_t capacity_;
    size_t size_;
    size_t front_index_;
  };

  template< typename T >
  DynamicArray< T >::DynamicArray():
    massive_(nullptr),
    capacity_(0),
    size_(0),
    front_index_(0)
  {}

  template< typename T >
  DynamicArray< T >::DynamicArray(const this_t & rhs):
    massive_(nullptr),
    capacity_(rhs.capacity_),
    size_(rhs.size_),
    front_index_(rhs.front_index_)
  {
    massive_ = new T[rhs.capacity_];
    try
    {
      for (size_t i = 0; i < rhs.size_; i++)
      {
        massive_[i] = rhs.massive_[i];
      }
    }
    catch (...)
    {
      delete massive_;
      throw;
    }
  }

  template< typename T >
  DynamicArray< T >::DynamicArray(this_t && rhs):
    massive_(rhs.massive_),
    capacity_(rhs.capacity_),
    size_(rhs.size_),
    front_index_(rhs.front_index_)
  {
    rhs.massive_ = nullptr;
  }

  template< typename T >
  DynamicArray< T >::~DynamicArray()
  {
    delete[] massive_;
  }

  template< typename T >
  typename DynamicArray< T >::this_t & DynamicArray< T >::operator=(const this_t & rhs)
  {
    DynamicArray cpy(rhs);
    swap(cpy);
    return *this;
  }

  template< typename T >
  typename DynamicArray< T >::this_t & DynamicArray< T >::operator=(this_t && rhs)
  {
    delete[] massive_;
    massive_ = rhs.massive_;
    capacity_ = rhs.capacity_;
    size_ = rhs.size_;
    front_index_ = rhs.front_index_;
    rhs.massive_ = nullptr;
    return *this;
  }

  template< typename T >
  T & DynamicArray< T >::operator[](const size_t & pos)
  {
    return const_cast< T & >(static_cast< const DynamicArray< T > * >(this)->operator[](pos));
  }

  template< typename T >
  const T & DynamicArray< T >::operator[](const size_t & pos) const
  {
    return massive_[pos];
  }


  template< typename T >
  template< typename T1 >
  void DynamicArray< T >::push_back(T1 && val)
  {
    T * temp = nullptr;
    try
    {
      if (empty())
      {
        temp = new T[5];
        temp[size_] = std::forward< T1 >(val);
        delete[] massive_;
        massive_ = temp;
        capacity_ = 5;
        size_++;
        front_index_ = 0;
      }
      else if ((size_ + front_index_) == capacity_)
      {
        temp = new T[capacity_ * 2];
        size_t i = 0;
        size_t j = front_index_;
        while (i < size_)
        {
          temp[i] = massive_[j];
          ++i;
          ++j;
        }
        temp[i] = std::forward< T1 >(val);
        delete[] massive_;
        massive_ = temp;
        capacity_ *= 2;
        size_++;
        front_index_ = 0;
      }
      else
      {
        massive_[size_ + front_index_] = std::forward< T1 >(val);
        size_++;
      }
    }
    catch (...)
    {
      delete[] temp;
      throw;
    }
  }

  template< typename T >
  void DynamicArray< T >::pop_back()
  {
    if (empty())
    {
      return;
    }
    size_--;
  }

  template< typename T >
  void DynamicArray< T >::pop_front()
  {
    if (empty())
    {
      return;
    }
    front_index_++;
    size_--;
  }

  template< typename T >
  T & DynamicArray< T >::front()
  {
    return massive_[front_index_];
  }

  template< typename T >
  const T & DynamicArray< T >::front() const
  {
    return massive_[front_index_];
  }

  template< typename T >
  T & DynamicArray< T >::back()
  {
    return massive_[size_ + front_index_ - 1];
  }

  template< typename T >
  const T & DynamicArray< T >::back() const
  {
    return massive_[size_ + front_index_ - 1];
  }

  template< typename T >
  bool DynamicArray< T >::empty() const noexcept
  {
    return !size_;
  }

  template< typename T >
  size_t DynamicArray< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void DynamicArray< T >::swap(this_t & rhs) noexcept
  {
    std::swap(massive_, rhs.massive_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_, rhs.size_);
  }
}

#endif
