#ifndef VECTOR_DECLARATION_HPP
#define VECTOR_DECLARATION_HPP

#include <cstddef>
#include "vector_iterator.hpp"

namespace maslevtsov {
  template< class T >
  class Vector
  {
  public:
    using iterator = VectorIterator< T, detail::VectorIteratorType::NONCONSTANT >;
    using const_iterator = VectorIterator< T, detail::VectorIteratorType::CONSTANT >;

    Vector();
    Vector(const Vector& rhs);
    Vector(Vector&& rhs) noexcept;
    explicit Vector(size_t size);
    ~Vector();

    Vector& operator=(const Vector& rhs);
    Vector& operator=(Vector&& rhs) noexcept;

    T& operator[](size_t pos) noexcept;
    const T& operator[](size_t pos) const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const noexcept;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    iterator erase(iterator pos);
    iterator erase(const_iterator pos);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back() noexcept;
    void swap(Vector& other) noexcept;

  private:
    size_t capacity_;
    size_t size_;
    T* data_;

    void expand(size_t count);
  };
}

#endif
