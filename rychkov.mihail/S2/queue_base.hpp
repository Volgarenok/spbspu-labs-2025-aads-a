#ifndef QUEUE_BASE_HPP
#define QUEUE_BASE_HPP

#include <cstddef>
#include <utility>

namespace rychkov
{
  namespace details
  {
    template< class T, bool PopFromTail >
    class QueueBase
    {
    public:
      using value_type = T;
      using size_type = size_t;
      using reference = value_type&;
      using const_reference = const value_type&;

      QueueBase() noexcept;
      QueueBase(const QueueBase& rhs);
      QueueBase(QueueBase&& rhs) noexcept;
      ~QueueBase();

      QueueBase& operator=(const QueueBase& rhs);
      QueueBase& operator=(QueueBase&& rhs) noexcept;

      reference operator[](size_type i);
      const_reference operator[](size_type i) const;

      bool empty() const noexcept;
      size_type size() const noexcept;
      size_type capacity() const noexcept;

      template< class... Args >
      reference emplace(Args&&... args);
      void push(const value_type& value);
      void push(value_type&& value);
      void pop();
      void clear();

      void reserve(size_type newCapacity);
      void swap(QueueBase& rhs) noexcept;
    private:
      size_type size_, capacity_, head_;
      value_type* data_;
      size_type getLocalId(size_type globalId) const noexcept;
    };
  }
}

template< class T, bool PopFromTail >
rychkov::details::QueueBase< T, PopFromTail >::QueueBase() noexcept:
  size_(0),
  capacity_(0),
  head_(0),
  data_(nullptr)
{}
template< class T, bool PopFromTail >
rychkov::details::QueueBase< T, PopFromTail >::QueueBase(const QueueBase& rhs):
  size_(0),
  capacity_(rhs.capacity_),
  head_(0),
  data_(new value_type[rhs.capacity_])
{
  for (size_type i = 0; i < rhs.size_; i++)
  {
    push(rhs[i]);
  }
}
template< class T, bool PopFromTail >
rychkov::details::QueueBase< T, PopFromTail >::QueueBase(QueueBase&& rhs) noexcept:
  size_(std::exchange(rhs.size_, 0)),
  capacity_(std::exchange(rhs.capacity_, 0)),
  head_(rhs.head_),
  data_(std::exchange(rhs.data_, nullptr))
{}
template< class T, bool PopFromTail >
rychkov::details::QueueBase< T, PopFromTail >::~QueueBase()
{
  clear();
}
template< class T, bool PopFromTail >
rychkov::details::QueueBase< T, PopFromTail >&
    rychkov::details::QueueBase< T, PopFromTail >::operator=(const QueueBase& rhs)
{
  QueueBase temp(rhs);
  swap(temp);
  return *this;
}
template< class T, bool PopFromTail >
rychkov::details::QueueBase< T, PopFromTail >&
    rychkov::details::QueueBase< T, PopFromTail >::operator=(QueueBase&& rhs) noexcept
{
  QueueBase temp(std::move(*this));
  swap(rhs);
  return *this;
}

template< class T, bool PopFromTail >
bool rychkov::details::QueueBase< T, PopFromTail >::empty() const noexcept
{
  return size_ == 0;
}
template< class T, bool PopFromTail >
typename rychkov::details::QueueBase< T, PopFromTail >::size_type
    rychkov::details::QueueBase< T, PopFromTail >::size() const noexcept
{
  return size_;
}
template< class T, bool PopFromTail >
typename rychkov::details::QueueBase< T, PopFromTail >::size_type
    rychkov::details::QueueBase< T, PopFromTail >::capacity() const noexcept
{
  return capacity_;
}
template< class T, bool PopFromTail >
void rychkov::details::QueueBase< T, PopFromTail >::pop()
{
  if (PopFromTail)
  {
    operator[](--size_).~value_type();
  }
  else
  {
    data_[head_].~value_type();
    size_--;
    head_ = (++head_ < capacity_ ? head_ : head_ - capacity_);
  }
}
template< class T, bool PopFromTail >
void rychkov::details::QueueBase< T, PopFromTail >::clear()
{
  while (!empty())
  {
    pop();
  }
  delete[] data_;
  capacity_ = 0;
  head_ = 0;
  data_ = nullptr;
}
template< class T, bool PopFromTail >
void rychkov::details::QueueBase< T, PopFromTail >::push(const value_type& value)
{
  emplace(value);
}
template< class T, bool PopFromTail >
void rychkov::details::QueueBase< T, PopFromTail >::push(value_type&& value)
{
  emplace(std::move(value));
}
template< class T, bool PopFromTail >
template< class... Args >
typename rychkov::details::QueueBase< T, PopFromTail >::reference
    rychkov::details::QueueBase< T, PopFromTail >::emplace(Args&&... args)
{
  if (size_ >= capacity_)
  {
    reserve(size_ * 2 + 1);
  }
  new (data_ + getLocalId(size_)) value_type(args...);
  return operator[](size_++);
}
template< class T, bool PopFromTail >
void rychkov::details::QueueBase< T, PopFromTail >::swap(QueueBase& rhs) noexcept
{
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
  std::swap(head_, rhs.head_);
  std::swap(data_, rhs.data_);
}
template< class T, bool PopFromTail >
void rychkov::details::QueueBase< T, PopFromTail >::reserve(size_type newCapacity)
{
  if (capacity_ >= newCapacity)
  {
    return;
  }
  QueueBase temp;
  temp.capacity_ = newCapacity;
  temp.data_ = new value_type[newCapacity];
  for (size_type i = 0; i < size_; i++)
  {
    temp.push(std::move_if_noexcept(operator[](i)));
  }
  swap(temp);
}
template< class T, bool PopFromTail >
typename rychkov::details::QueueBase< T, PopFromTail >::size_type
    rychkov::details::QueueBase< T, PopFromTail >::getLocalId(size_type globalId) const noexcept
{
  globalId += head_;
  return (globalId < capacity_ ? globalId : globalId - capacity_);
}
template< class T, bool PopFromTail >
typename rychkov::details::QueueBase< T, PopFromTail >::reference
    rychkov::details::QueueBase< T, PopFromTail >::operator[](size_type i)
{
  return data_[getLocalId(i)];
}
template< class T, bool PopFromTail >
typename rychkov::details::QueueBase< T, PopFromTail >::const_reference
    rychkov::details::QueueBase< T, PopFromTail >::operator[](size_type i) const
{
  return data_[getLocalId(i)];
}

#endif
