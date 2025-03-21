#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <utility>
#include <cstddef>

namespace
{
  template< typename T >
  T* getDataFromOther(const T* data, const size_t size)
  {
    T* new_data = new T[size];
    try
    {
      for (size_t i = 0; i < size; ++i)
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
  class DynArray
  {
  public:
    DynArray();
    DynArray(const DynArray& other);
    DynArray(DynArray&& other) noexcept;
    ~DynArray();
    void pushBack(T&& data);
    void pushBack(const T& data);
    void popBack();
    void popFront();
    size_t size() const noexcept;
    bool empty() const noexcept;
    T& back();
    const T& back() const;
    T& front();
    const T& front() const;
    void swap(DynArray& other) noexcept;
  private:
    size_t size_;
    T* data_;
    template< class Q>
    void push(Q&& data);
  };

  template< typename T >
  DynArray< T >::DynArray():
    size_(0),
    data_(nullptr)
  {}

  template< typename T >
  DynArray< T >::DynArray(const DynArray& other):
    size_(other.size_),
    data_(getDataFromOther(other.data_, other.size_))
  {}

  template< typename T >
  DynArray< T >::DynArray(DynArray&& other) noexcept:
    size_(other.size_),
    data_(other.data_)
  {
    other.size_ = 0;
    other.data_ = nullptr;
  }

  template< typename T >
  DynArray< T >::~DynArray()
  {
    delete[] data_;
  }

  template< typename T >
  void DynArray< T >::pushBack(T&& data)
  {
    push(std::move(data));
  }

  template< typename T >
  void DynArray< T >::pushBack(const T& data)
  {
    push(data);
  }

  template< typename T >
  template< class Q>
  void DynArray< T >::push(Q&& data)
  {
    T* new_data = new T[size_ + 1];
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        new_data[i] = data_[i];
      }
      new_data[size_] = std::forward< Q >(data);
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
    delete[] data_;
    size_++;
    data_ = new_data;
  }

  template< typename T >
  bool DynArray< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t DynArray< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void DynArray< T >::swap(DynArray< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  T& DynArray< T >::back()
  {
    return const_cast< T& >(static_cast< const DynArray< T >* >(this)->back());
  }

  template< typename T >
  const T& DynArray< T >::back() const
  {
    return data_[size_ - 1];
  }

  template< typename T >
  T& DynArray< T >::front()
  {
    return const_cast< T& >(static_cast< const DynArray< T >* >(this)->front());
  }

  template< typename T >
  void DynArray< T >::popFront()
  {
    T* new_data = new T[size_ - 1];;
    try
    {
      for (size_t i = 1; i < size_; ++i)
      {
        new_data[i - 1] = data_[i];
      }
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
    delete[] data_;
    size_--;
    data_ = new_data;
  }

  template< typename T >
  void DynArray< T >::popBack()
  {
     T* new_data = new T[size_ - 1];;
    try
    {
      for (size_t i = 0; i < size_ - 1; ++i)
      {
        new_data[i] = data_[i];
      }
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
    delete[] data_;
    size_--;
    data_ = new_data;
  }

  template< typename T >
  const T& DynArray< T >::front() const
  {
    return data_[0];
  }

}
#endif
