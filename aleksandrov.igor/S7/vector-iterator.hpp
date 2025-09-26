#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include <cassert>
#include <iterator>

namespace aleksandrov
{
  template< class T >
  class Vector;

  template< class T, bool isConst >
  class VectorIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    using Reference = std::conditional_t< isConst, const T&, T& >;
    using Pointer = std::conditional_t< isConst, const T*, T* >;

    VectorIterator();

    VectorIterator& operator++() noexcept;
    VectorIterator operator++(int) noexcept;
    VectorIterator& operator--() noexcept;
    VectorIterator operator--(int) noexcept;

    Reference operator*() const noexcept;
    Pointer operator->() const noexcept;

    bool operator==(const VectorIterator&) const noexcept;
    bool operator!=(const VectorIterator&) const noexcept;

  private:
    friend class Vector< T >;

    T* ptr_;

    explicit VectorIterator(T*);
  };

  template< class T, bool isConst >
  VectorIterator< T, isConst >::VectorIterator():
    ptr_(nullptr)
  {}

  template< class T, bool isConst >
  VectorIterator< T, isConst >::VectorIterator(T* ptr):
    ptr_(ptr)
  {}

  template< class T, bool isConst >
  VectorIterator< T, isConst >& VectorIterator< T, isConst >::operator++() noexcept
  {
    assert(ptr_ != nullptr && "Trying to iterate from null-ptr!");
    ++ptr_;
    return *this;
  }

  template< class T, bool isConst >
  VectorIterator< T, isConst > VectorIterator< T, isConst >::operator++(int) noexcept
  {
    assert(ptr_ != nullptr && "Trying to iterate from null-ptr!");
    auto result(*this);
    ++(*this);
    return result;
  }

  template< class T, bool isConst >
  VectorIterator< T, isConst >& VectorIterator< T, isConst >::operator--() noexcept
  {
    assert(ptr_ != nullptr && "Trying to iterate from null-ptr!");
    --ptr_;
    return *this;
  }

  template< class T, bool isConst >
  VectorIterator< T, isConst > VectorIterator< T, isConst >::operator--(int) noexcept
  {
    assert(ptr_ != nullptr && "Trying to iterate from null-ptr!");
    auto result(*this);
    --(*this);
    return result;
  }

  template< class T, bool isConst >
  typename VectorIterator< T, isConst >::Reference VectorIterator< T, isConst >::operator*() const noexcept
  {
    assert(ptr_ != nullptr && "Access to null-ptr!");
    return *ptr_;
  }

  template< class T, bool isConst >
  typename VectorIterator< T, isConst >::Pointer VectorIterator< T, isConst >::operator->(
) const noexcept
  {
    assert(ptr_ != nullptr && "Access to null-ptr!");
    return ptr_;
  }

  template< class T, bool isConst >
  bool VectorIterator< T, isConst >::operator==(const VectorIterator& rhs) const noexcept
  {
    return ptr_ == rhs.ptr_;
  }

  template< class T, bool isConst >
  bool VectorIterator< T, isConst >::operator!=(const VectorIterator& rhs) const noexcept
  {
    return !operator==(rhs);
  }
}

#endif

