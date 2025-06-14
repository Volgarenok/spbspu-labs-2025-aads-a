#ifndef DYNAMICARR_HPP
#define DYNAMICARR_HPP
#include <cstddef>
#include <stdexcept>

namespace finaev
{
  template< class T >
  class dynamicArr
  {
  public:
    dynamicArr();
    ~dynamicArr();
    dynamicArr(const dynamicArr< T >&);
    dynamicArr(dynamicArr< T >&&) noexcept;

    void swap(dynamicArr< T >&) noexcept;

    dynamicArr& operator=(const dynamicArr< T >& other);
    dynamicArr& operator=(dynamicArr< T >&& other) noexcept;

    void push(const T&);
    void pop_back();
    void pop_front();

    size_t size() const noexcept;
    bool empty() const noexcept;

    const T& front() const;
    T& front();

    const T& back() const;
    T& back();
  private:
    size_t capacity_;
    size_t size_;
    size_t head_;
    T* data_;

    void resize();
  };

  template< class T >
  dynamicArr< T >::dynamicArr():
    capacity_(1),
    size_(0),
    head_(0),
    data_(new T[capacity_])
  {}

  template< class T >
  dynamicArr< T >::~dynamicArr()
  {
    delete[] data_;
    data_ = nullptr;
  }

  template< class T >
  dynamicArr< T >::dynamicArr(const dynamicArr< T >& other):
    capacity_(other.capacity_),
    size_(other.size_),
    head_(other.head_),
    data_(new T[other.capacity_])
  {
    if (capacity_ > 0)
    {
      try
      {
        for (size_t i = 0; i < size_; ++i)
        {
          data_[i] = other.data_[i];
        }
      }
      catch (...)
      {
        delete[] data_;
        throw;
      }
    }
  }

  template< class T >
  dynamicArr< T >::dynamicArr(dynamicArr< T >&& other) noexcept:
    capacity_(other.capacity_),
    size_(other.size_),
    head_(other.head_),
    data_(other.data_)
  {
    other.capacity_ = 0;
    other.size_ = 0;
    other.data_ = nullptr;
  }

  template< class T >
  void dynamicArr< T >::swap(dynamicArr< T >& other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
    std::swap(data_, other.data_);
  }

  template< class T >
  dynamicArr< T >& dynamicArr< T >::operator=(const dynamicArr< T >& other)
  {
    if (this != std::addressof(other))
    {
      dynamicArr< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class T >
  dynamicArr< T >& dynamicArr< T >::operator=(dynamicArr< T >&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      dynamicArr< T > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< class T >
  void dynamicArr< T >::resize()
  {
    size_t newCapacity = capacity_ * 2;
    T* newData = nullptr;
    try
    {
      newData = new T[newCapacity];
      for (size_t i = 0; i < size_; ++i)
      {
        size_t index = (head_ + i) % capacity_;
        newData[i] = data_[index];
      }
    }
    catch (...)
    {
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
    head_ = 0;
  }

  template< class T >
  void dynamicArr< T >::push(const T& value)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    data_[size_ + head_] = value;
    ++size_;
  }

  template< class T >
  void dynamicArr< T >::pop_back()
  {
    if (empty())
    {
      throw std::out_of_range("can not pop, cont is empty!");
    }
    --size_;
  }

  template< class T >
  void dynamicArr< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("can not pop, cont is empty!");
    }
    head_ = (head_ + 1) % capacity_;
    --size_;
  }

  template< class T >
  bool dynamicArr< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t dynamicArr< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  const T& dynamicArr< T >::front() const
  {
    return data_[head_];
  }

  template< class T >
  T& dynamicArr< T >::front()
  {
    return data_[head_];
  }

  template< class T >
  const T& dynamicArr< T >::back() const
  {
    return data_[size_ + head_ - 1];
  }

  template< class T >
  T& dynamicArr< T >::back()
  {
    return data_[size_ + head_ - 1];
  }
}

#endif
