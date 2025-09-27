#ifndef ITERATOR_ARRAY_HPP
#define ITERATOR_ARRAY_HPP
#include <array.hpp>

namespace averenkov
{

  template< typename T, bool IsConst >
  class ArrayIterator
  {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t< IsConst, const T, T >;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    ArrayIterator();
    explicit ArrayIterator(pointer ptr);

    template< bool OtherIsConst, typename = std::enable_if_t< IsConst && !OtherIsConst > >
    ArrayIterator(const ArrayIterator< T, OtherIsConst >& other);

    reference operator*() const;
    pointer operator->() const;

    ArrayIterator& operator++();
    ArrayIterator operator++(int);
    ArrayIterator& operator--();
    ArrayIterator operator--(int);

    ArrayIterator& operator+=(difference_type n);
    ArrayIterator& operator-=(difference_type n);

    ArrayIterator operator+(difference_type n) const;
    ArrayIterator operator-(difference_type n) const;
    difference_type operator-(const ArrayIterator& other) const;

    bool operator==(const ArrayIterator& other) const;
    bool operator!=(const ArrayIterator& other) const;
    bool operator<(const ArrayIterator& other) const;
    bool operator>(const ArrayIterator& other) const;
    bool operator<=(const ArrayIterator& other) const;
    bool operator>=(const ArrayIterator& other) const;

  private:
    pointer ptr_;

    friend class ArrayIterator< T, true >;
    friend class ArrayIterator< T, false >;

    template< class U >
    friend class Array;
  };

}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>::ArrayIterator():
  ptr_(nullptr)
{}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>::ArrayIterator(pointer ptr):
  ptr_(ptr)
{}

template< typename T, bool IsConst >
template< bool OtherIsConst, typename >
averenkov::ArrayIterator<T, IsConst>::ArrayIterator(const ArrayIterator<T, OtherIsConst>& other):
  ptr_(other.ptr_)
{}

template< typename T, bool IsConst >
typename averenkov::ArrayIterator<T, IsConst>::reference
averenkov::ArrayIterator<T, IsConst>::operator*() const
{
  return *ptr_;
}

template< typename T, bool IsConst >
typename averenkov::ArrayIterator<T, IsConst>::pointer
averenkov::ArrayIterator<T, IsConst>::operator->() const
{
  return ptr_;
}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>&
averenkov::ArrayIterator<T, IsConst>::operator++()
{
  ++ptr_;
  return *this;
}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>
averenkov::ArrayIterator<T, IsConst>::operator++(int)
{
  ArrayIterator temp = *this;
  ++ptr_;
  return temp;
}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>&
averenkov::ArrayIterator<T, IsConst>::operator--()
{
  --ptr_;
  return *this;
}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>
averenkov::ArrayIterator<T, IsConst>::operator--(int)
{
  ArrayIterator temp = *this;
  --ptr_;
  return temp;
}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>&
averenkov::ArrayIterator<T, IsConst>::operator+=(difference_type n)
{
  ptr_ += n;
  return *this;
}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>&
averenkov::ArrayIterator<T, IsConst>::operator-=(difference_type n)
{
  ptr_ -= n;
  return *this;
}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>
averenkov::ArrayIterator<T, IsConst>::operator+(difference_type n) const
{
  return ArrayIterator(ptr_ + n);
}

template< typename T, bool IsConst >
averenkov::ArrayIterator<T, IsConst>
averenkov::ArrayIterator<T, IsConst>::operator-(difference_type n) const
{
  return ArrayIterator(ptr_ - n);
}

template< typename T, bool IsConst >
typename averenkov::ArrayIterator<T, IsConst>::difference_type
averenkov::ArrayIterator<T, IsConst>::operator-(const ArrayIterator& other) const
{
  return ptr_ - other.ptr_;
}

template< typename T, bool IsConst >
bool averenkov::ArrayIterator<T, IsConst>::operator==(const ArrayIterator& other) const
{
  return ptr_ == other.ptr_;
}

template< typename T, bool IsConst >
bool averenkov::ArrayIterator<T, IsConst>::operator!=(const ArrayIterator& other) const
{
  return ptr_ != other.ptr_;
}

template< typename T, bool IsConst >
bool averenkov::ArrayIterator<T, IsConst>::operator<(const ArrayIterator& other) const
{
  return ptr_ < other.ptr_;
}

template< typename T, bool IsConst >
bool averenkov::ArrayIterator<T, IsConst>::operator>(const ArrayIterator& other) const
{
  return ptr_ > other.ptr_;
}

template< typename T, bool IsConst >
bool averenkov::ArrayIterator<T, IsConst>::operator<=(const ArrayIterator& other) const
{
  return ptr_ <= other.ptr_;
}

template< typename T, bool IsConst >
bool averenkov::ArrayIterator<T, IsConst>::operator>=(const ArrayIterator& other) const
{
  return ptr_ >= other.ptr_;
}

#endif
