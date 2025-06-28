#ifndef DYNAMIC_ARRAY_ITERATOR_HPP
#define DYNAMIC_ARRAY_ITERATOR_HPP

namespace demehin
{
  template< typename T >
  class DynamicArray;

  template< typename T >
  class DynamicArrayIterator
  {
    friend class DynamicArray< T >;
  public:

    T& operator*() const noexcept;
    T* operator->() const noexcept;

    DynamicArrayIterator& operator++() noexcept;
    DynamicArrayIterator operator++(int) noexcept;

    DynamicArrayIterator& operator--() noexcept;
    DynamicArrayIterator operator--(int) noexcept;

    bool operator==(const DynamicArrayIterator&) const noexcept;
    bool operator!=(const DynamicArrayIterator&) const noexcept;

  private:

    T* ptr_;
    explicit DynamicArrayIterator(T*) noexcept;
  };

  template< typename T >
  DynamicArrayIterator< T >::DynamicArrayIterator(T* ptr) noexcept:
    ptr_(ptr)
  {}

  template< typename T >
  T& DynamicArrayIterator< T >::operator*() const noexcept
  {
    return *ptr_;
  }

  template< typename T >
  T* DynamicArrayIterator< T >::operator->() const noexcept
  {
    return ptr_;
  }

  template< typename T >
  DynamicArrayIterator< T >& DynamicArrayIterator< T >::operator++() noexcept
  {
    ++ptr_;
    return *this;
  }

  template< typename T >
  DynamicArrayIterator< T > DynamicArrayIterator< T >::operator++(int) noexcept
  {
    DynamicArrayIterator tmp = *this;
    ++ptr_;
    return tmp;
  }

  template< typename T >
  DynamicArrayIterator< T >& DynamicArrayIterator< T >::operator--() noexcept
  {
    --ptr_;
    return *this;
  }

  template< typename T >
  DynamicArrayIterator< T > DynamicArrayIterator< T >::operator--(int) noexcept
  {
    DynamicArrayIterator tmp = *this;
    --ptr_;
    return tmp;
  }

  template< typename T >
  bool DynamicArrayIterator< T >::operator==(const DynamicArrayIterator& rhs) const noexcept
  {
    return ptr_ == rhs.ptr_;
  }

  template< typename T >
  bool DynamicArrayIterator< T >::operator!=(const DynamicArrayIterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}

#endif
