#ifndef DYNAMIC_ARRAY_CITERATOR_HPP
#define DYNAMIC_ARRAY_CITERATOR_HPP
#include "dynamic_array_iterator.hpp"

namespace demehin
{

  template< typename T >
  class DynamicArray;

  template< typename T >
  class DynamicArrayIterator;

  template< typename T >
  class DynamicArraycIterator
  {
    friend class DynamicArray< T >;
  public:

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    DynamicArraycIterator& operator++() noexcept;
    DynamicArraycIterator operator++(int) noexcept;

    DynamicArraycIterator& operator--() noexcept;
    DynamicArraycIterator operator--(int) noexcept;

    bool operator==(const DynamicArraycIterator&) const noexcept;
    bool operator!=(const DynamicArraycIterator&) const noexcept;

    DynamicArraycIterator operator-(size_t) const noexcept;
    size_t operator-(const DynamicArraycIterator&) const noexcept;


  private:

    const T* ptr_;
    explicit DynamicArraycIterator(const T*) noexcept;
  };

  template< typename T >
  DynamicArraycIterator< T >::DynamicArraycIterator(const T* ptr) noexcept:
    ptr_(ptr)
  {}


  template< typename T >
  const T& DynamicArraycIterator< T >::operator*() const noexcept
  {
    return *ptr_;
  }

  template< typename T >
  const T* DynamicArraycIterator< T >::operator->() const noexcept
  {
    return ptr_;
  }

  template< typename T >
  DynamicArraycIterator< T >& DynamicArraycIterator< T >::operator++() noexcept
  {
    ++ptr_;
    return *this;
  }

  template< typename T >
  DynamicArraycIterator< T > DynamicArraycIterator< T >::operator++(int) noexcept
  {
    DynamicArraycIterator tmp = *this;
    ++ptr_;
    return tmp;
  }

  template< typename T >
  DynamicArraycIterator< T >& DynamicArraycIterator< T >::operator--() noexcept
  {
    --ptr_;
    return *this;
  }

  template< typename T >
  DynamicArraycIterator< T > DynamicArraycIterator< T >::operator--(int) noexcept
  {
    DynamicArraycIterator tmp = *this;
    --ptr_;
    return tmp;
  }

  template <typename T>
  DynamicArraycIterator<T> DynamicArraycIterator<T>::operator-(size_t n) const noexcept
  {
    return DynamicArraycIterator(ptr_ - n);
  }

  template <typename T>
  size_t DynamicArraycIterator<T>::operator-(const DynamicArraycIterator& rhs) const noexcept
  {
    return ptr_ - rhs.ptr_;
  }

  template< typename T >
  bool DynamicArraycIterator< T >::operator==(const DynamicArraycIterator& rhs) const noexcept
  {
    return ptr_ == rhs.ptr_;
  }

  template< typename T >
  bool DynamicArraycIterator< T >::operator!=(const DynamicArraycIterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }

}

#endif
