#ifndef DYNAMICARR_HPP
#define DYNAMICARR_HPP
#include <stdexcept>
#include <utility>

namespace kiselev
{
  template< typename T >
  class DynamicArr
  {
  public:
    DynamicArr();
    DynamicArr(const DynamicArr< T >&);
    DynamicArr(DynamicArr< T >&&) noexcept;
    DynamicArr(size_t capacity);
    ~DynamicArr();

    DynamicArr< T >& operator=(const DynamicArr< T >&);
    DynamicArr< T >& operator=(DynamicArr< T >&&) noexcept;

    T& back();
    const T& back() const;
    T& front();
    const T& front() const;
    void popBack();
    void popFront();
    void push(const T&) noexcept;
    void push(T&&) noexcept;
    void clear() noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

  private:
    T** data_;
    size_t capacity_;
    size_t size_;
    size_t begin_;

    void reallocate();
    void swap(DynamicArr< T >&) noexcept;
  };

  template< typename T >
  void DynamicArr< T >::reallocate()
  {
    size_t newCapacity = capacity_ * 2;
    DynamicArr< T > newArr(newCapacity);
    for (size_t i = 0; i < size_; ++i)
    {
      newArr.push(*data_[i + begin_]);
    }
    swap(newArr);
  }

  template< typename T >
  void DynamicArr< T >::clear() noexcept
  {
    while (!empty())
    {
      popBack();
    }
    size_ = 0;
    begin_ = 0;
  }
  template< typename T >
  void DynamicArr< T >::swap(DynamicArr< T >& arr) noexcept
  {
    std::swap(data_, arr.data_);
    std::swap(capacity_, arr.capacity_);
    std::swap(size_, arr.size_);
    std::swap(begin_, arr.begin_);
  }

  template< typename T >
  DynamicArr< T >::DynamicArr():
    data_(nullptr),
    capacity_(5),
    size_(0),
    begin_(0)
  {
    data_ = new T*[capacity_]();
  }

  template< typename T >
  DynamicArr< T >::DynamicArr(const DynamicArr< T >& arr):
    data_(new T*[arr.capacity_]()),
    capacity_(arr.capacity_),
    size_(0),
    begin_(0)
  {
    try
    {
      for (; size_ < arr.size(); ++size_)
      {
        data_[size_] = new T(*arr.data_[size_ + arr.begin_]);
      }
    }
    catch (...)
    {
      clear();
      delete[] data_;
      throw;
    }
  }

  template< typename T >
  DynamicArr< T >::DynamicArr(DynamicArr< T >&& arr) noexcept:
    data_(std::exchange(arr.data_, nullptr)),
    capacity_(std::exchange(arr.capacity_, 0)),
    size_(std::exchange(arr.size_, 0)),
    begin_(std::exchange(arr.begin_, 0))
  {}

  template< typename T >
  DynamicArr< T >::DynamicArr(size_t capacity):
    data_(new T*[capacity]()),
    capacity_(capacity),
    size_(0),
    begin_(0)
  {}

  template< typename T >
  DynamicArr< T >::~DynamicArr()
  {
    clear();
    delete[] data_;
    capacity_ = 0;
  }

  template< typename T >
  DynamicArr< T >& DynamicArr< T >::operator=(const DynamicArr< T >& arr)
  {
    DynamicArr< T > copy(arr);
    swap(copy);
    return *this;
  }

  template< typename T >
  DynamicArr< T >& DynamicArr< T >::operator=(DynamicArr< T >&& arr) noexcept
  {
    DynamicArr< T > temp(std::move(arr));
    swap(temp);
    return *this;
  }

  template< typename T >
  T& DynamicArr< T >::back()
  {
    return const_cast< T& >(static_cast< const DynamicArr< T >& >(*this).back());
  }

  template< typename T >
  const T& DynamicArr< T >::back() const
  {
    if (empty())
    {
      throw std::logic_error("Empty for back()");
    }
    return *(data_[begin_ + size_ - 1]);
  }

  template< typename T >
  const T& DynamicArr< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("Empty for front()");
    }
    return *(data_[begin_]);
  }

  template< typename T >
  T& DynamicArr< T >::front()
  {
    return const_cast< T& >(static_cast< const DynamicArr< T >& >(*this).front());
  }

  template< typename T >
  void DynamicArr< T >::popBack()
  {
    if (empty())
    {
      throw std::logic_error("Empty for popBack()");
    }
    delete data_[begin_ + size_ - 1];
    data_[begin_ + size_ - 1] = nullptr;
    --size_;
  }

  template< typename T >
  void DynamicArr< T >::popFront()
  {
    if (empty())
    {
      throw std::logic_error("Empty for popFront()");
    }
    delete data_[begin_];
    data_[begin_] = nullptr;
    ++begin_;
    --size_;
    if (empty())
    {
      begin_ = 0;
    }
  }

  template< typename T >
  void DynamicArr< T >::push(const T& data) noexcept
  {
    if (size_ >= capacity_)
    {
      reallocate();
    }
    data_[begin_ + size_] = new T(data);
    ++size_;
  }

  template< typename T >
  void DynamicArr< T >::push(T&& data) noexcept
  {
    if (size_ >= capacity_)
    {
      reallocate();
    }
    data_[begin_ + size_] = new T(std::move(data));
    ++size_;
  }

  template< typename T >
  size_t DynamicArr< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  bool DynamicArr< T >::empty() const noexcept
  {
    return size_ == 0;
  }

}

#endif
