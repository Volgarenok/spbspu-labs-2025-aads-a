#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include <iterator>
#include <type_traits>

namespace maslevtsov {
  namespace detail {
    enum class VectorIteratorType
    {
      CONSTANT,
      NONCONSTANT,
    };
  }

  template< class T, detail::VectorIteratorType it_type >
  class VectorIterator final: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    template< class Type >
    friend class Vector;

    VectorIterator& operator++() noexcept;
    VectorIterator& operator++(int) noexcept;
    VectorIterator& operator--() noexcept;
    VectorIterator& operator--(int) noexcept;

    typename std::conditional< it_type == detail::VectorIteratorType::CONSTANT, const T&, T& >::type
      operator*() const noexcept;
    typename std::conditional< it_type == detail::VectorIteratorType::CONSTANT, const T*, T* >::type
      operator->() const noexcept;

    bool operator==(const VectorIterator& rhs) const noexcept;
    bool operator!=(const VectorIterator& rhs) const noexcept;

  private:
    T* ptr_;

    explicit VectorIterator(T* ptr) noexcept;
  };

  template< class T, detail::VectorIteratorType it_type >
  typename VectorIterator< T, it_type >::VectorIterator& VectorIterator< T, it_type >::operator++() noexcept
  {
    ++ptr_;
    return *this;
  }

  template< class T, detail::VectorIteratorType it_type >
  typename VectorIterator< T, it_type >::VectorIterator& VectorIterator< T, it_type >::operator++(int) noexcept
  {
    VectorIterator< T, it_type > result(*this);
    ++(*this);
    return result;
  }

  template< class T, detail::VectorIteratorType it_type >
  typename VectorIterator< T, it_type >::VectorIterator& VectorIterator< T, it_type >::operator--() noexcept
  {
    --ptr_;
    return *this;
  }

  template< class T, detail::VectorIteratorType it_type >
  typename VectorIterator< T, it_type >::VectorIterator& VectorIterator< T, it_type >::operator--(int) noexcept
  {
    VectorIterator< T, it_type > result(*this);
    --(*this);
    return result;
  }

  template< class T, detail::VectorIteratorType it_type >
  typename std::conditional< it_type == detail::VectorIteratorType::CONSTANT, const T&, T& >::type
    VectorIterator< T, it_type >::operator*() const noexcept
  {
    return *ptr_;
  }

  template< class T, detail::VectorIteratorType it_type >
  typename std::conditional< it_type == detail::VectorIteratorType::CONSTANT, const T*, T* >::type
    VectorIterator< T, it_type >::operator->() const noexcept
  {
    return ptr_;
  }

  template< class T, detail::VectorIteratorType it_type >
  bool VectorIterator< T, it_type >::operator==(const VectorIterator& rhs) const noexcept
  {
    return ptr_ == rhs.ptr_;
  }

  template< class T, detail::VectorIteratorType it_type >
  bool VectorIterator< T, it_type >::operator!=(const VectorIterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class T, detail::VectorIteratorType it_type >
  VectorIterator< T, it_type >::VectorIterator(T* ptr) noexcept:
    ptr_(ptr)
  {}
}

#endif
