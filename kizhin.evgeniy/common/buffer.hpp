#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_BUFFER_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_BUFFER_HPP

#include <cassert>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <utility>
#include "algorithm-utils.hpp"
#include "type-utils.hpp"

namespace kizhin {
  template < typename T >
  class Buffer final
  {
  public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using size_type = std::size_t;

    Buffer() noexcept;
    Buffer(const Buffer&);
    Buffer(Buffer&&) noexcept;
    Buffer(size_type, const_reference = value_type{});
    template < typename InputIt, enable_if_input_iterator< InputIt > = 0 >
    Buffer(InputIt, InputIt);
    Buffer(std::initializer_list< value_type >);
    ~Buffer();

    Buffer& operator=(const Buffer&);
    Buffer& operator=(Buffer&&) noexcept;

    void assign(size_type, const_reference = value_type{});
    template < typename InputIt, enable_if_input_iterator< InputIt > = 0 >
    void assign(InputIt, InputIt);
    void assign(std::initializer_list< value_type >);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    size_type size() const noexcept;
    size_type capacity() const noexcept;
    size_type maxSize() const;
    bool empty() const noexcept;

    reference front() noexcept;
    reference back() noexcept;
    const_reference front() const noexcept;
    const_reference back() const noexcept;

    template < typename... Args >
    void emplaceFront(Args&&...);
    template < typename... Args >
    void emplaceBack(Args&&...);
    void pushBack(const_reference);
    void pushBack(value_type&&);
    void pushFront(const_reference);
    void pushFront(value_type&&);

    void popBack() noexcept;
    void popFront() noexcept;
    void clear() noexcept;
    void swap(Buffer&) noexcept;

  private:
    pointer begin_;
    pointer end_;
    pointer last_;

    void throw_length_error() const;

    void allocate(size_type);
    void deallocate() noexcept;
    void expand(size_type);
    size_type growthCapacity(size_type) const;

    template < typename InputIt, enable_if_input_iterator< InputIt > = 0 >
    void constructAtEnd(InputIt, InputIt);
    void constructAtEnd(size_type, const_reference = value_type());
    void destroyAtEnd(pointer) noexcept;
    void destroyAtBegin(pointer) noexcept;
  };

  template < typename T >
  Buffer< T >::Buffer() noexcept:
    begin_(nullptr),
    end_(nullptr),
    last_(nullptr)
  {}

  template < typename T >
  Buffer< T >::Buffer(const Buffer& rhs):
    Buffer()
  {
    allocate(rhs.size());
    constructAtEnd(rhs.begin(), rhs.end());
  }

  template < typename T >
  Buffer< T >::Buffer(Buffer&& rhs) noexcept:
    begin_(std::exchange(rhs.begin_, nullptr)),
    end_(std::exchange(rhs.end_, nullptr)),
    last_(std::exchange(rhs.last_, nullptr))
  {}

  template < typename T >
  Buffer< T >::Buffer(const size_type size, const_reference value):
    Buffer()
  {
    allocate(size);
    constructAtEnd(size, value);
  }

  template < typename T >
  template < typename InputIt, enable_if_input_iterator< InputIt > >
  Buffer< T >::Buffer(InputIt first, const InputIt last):
    Buffer()
  {
    for (; first != last; ++first) {
      pushBack(*first);
    }
  }

  template < typename T >
  Buffer< T >::Buffer(std::initializer_list< value_type > init):
    Buffer(init.begin(), init.end())
  {}

  template < typename T >
  Buffer< T >::~Buffer()
  {
    clear();
    deallocate();
  }

  template < typename T >
  Buffer< T >& Buffer< T >::operator=(const Buffer& rhs)
  {
    Buffer copy(rhs);
    swap(copy);
    return *this;
  }

  template < typename T >
  Buffer< T >& Buffer< T >::operator=(Buffer&& rhs) noexcept
  {
    clear();
    deallocate();
    swap(rhs);
    return *this;
  }

  template < typename T >
  void Buffer< T >::assign(const size_type size, const_reference value)
  {
    Buffer tmp(size, value);
    swap(tmp);
  }

  template < typename T >
  template < typename InputIt, enable_if_input_iterator< InputIt > >
  void Buffer< T >::assign(const InputIt first, const InputIt last)
  {
    Buffer tmp(first, last);
    swap(tmp);
  }

  template < typename T >
  void Buffer< T >::assign(std::initializer_list< value_type > init)
  {
    Buffer tmp(init);
    swap(tmp);
  }

  template < typename T >
  typename Buffer< T >::iterator Buffer< T >::begin() noexcept
  {
    return iterator(begin_);
  }

  template < typename T >
  typename Buffer< T >::iterator Buffer< T >::end() noexcept
  {
    return iterator(end_);
  }

  template < typename T >
  typename Buffer< T >::const_iterator Buffer< T >::begin() const noexcept
  {
    return const_iterator(begin_);
  }

  template < typename T >
  typename Buffer< T >::const_iterator Buffer< T >::end() const noexcept
  {
    return const_iterator(end_);
  }

  template < typename T >
  typename Buffer< T >::size_type Buffer< T >::size() const noexcept
  {
    return static_cast< size_type >(end_ - begin_);
  }

  template < typename T >
  typename Buffer< T >::size_type Buffer< T >::capacity() const noexcept
  {
    return static_cast< size_type >(last_ - begin_);
  }

  template < typename T >
  typename Buffer< T >::size_type Buffer< T >::maxSize() const
  {
    return std::numeric_limits< size_type >::max() / sizeof(T);
  }

  template < typename T >
  bool Buffer< T >::empty() const noexcept
  {
    return begin_ == end_;
  }

  template < typename T >
  typename Buffer< T >::reference Buffer< T >::front() noexcept
  {
    assert(!empty() && "front() called on empty SplitBuffer");
    return *begin_;
  }

  template < typename T >
  typename Buffer< T >::reference Buffer< T >::back() noexcept
  {
    assert(!empty() && "back() called on empty SplitBuffer");
    return *(end_ - 1);
  }

  template < typename T >
  typename Buffer< T >::const_reference Buffer< T >::front() const noexcept
  {
    assert(!empty() && "front() called on empty SplitBuffer");
    return *begin_;
  }

  template < typename T >
  typename Buffer< T >::const_reference Buffer< T >::back() const noexcept
  {
    assert(!empty() && "back() called on empty SplitBuffer");
    return *(end_ - 1);
  }

  template < typename T >
  template < typename... Args >
  void Buffer< T >::emplaceFront(Args&&... args)
  {
    if (end_ == last_) {
      expand(growthCapacity(capacity() + 1));
    }
    if (end_ != begin_) {
      new (end_) value_type(std::move(*(end_ - 1)));
      moveBackward(begin_, end_, end_ + 1);
      *begin_ = value_type(std::forward< Args >(args)...);
    } else {
      new (begin_) value_type(std::forward< Args >(args)...);
    }
    ++end_;
  }

  template < typename T >
  template < typename... Args >
  void Buffer< T >::emplaceBack(Args&&... args)
  {
    if (end_ == last_) {
      expand(growthCapacity(capacity() + 1));
    }
    new (end_) value_type(std::forward< Args >(args)...);
    ++end_;
  }

  template < typename T >
  void Buffer< T >::pushBack(const_reference value)
  {
    emplaceBack(value);
  }

  template < typename T >
  void Buffer< T >::pushBack(value_type&& value)
  {
    emplaceBack(std::move(value));
  }

  template < typename T >
  void Buffer< T >::pushFront(const_reference value)
  {
    emplaceFront(value);
  }

  template < typename T >
  void Buffer< T >::pushFront(value_type&& value)
  {
    emplaceFront(std::move(value));
  }

  template < typename T >
  void Buffer< T >::popBack() noexcept
  {
    assert(!empty() && "popBack called on empty SplitBuffer");
    destroyAtEnd(end_ - 1);
  }

  template < typename T >
  void Buffer< T >::popFront() noexcept
  {
    assert(!empty() && "popFront called on empty SplitBuffer");
    destroyAtBegin(begin_ + 1);
  }

  template < typename T >
  void Buffer< T >::clear() noexcept
  {
    destroyAtEnd(begin_);
  }

  template < typename T >
  void Buffer< T >::swap(Buffer& rhs) noexcept
  {
    using std::swap;
    swap(begin_, rhs.begin_);
    swap(end_, rhs.end_);
    swap(last_, rhs.last_);
  }

  template < typename T >
  void Buffer< T >::throw_length_error() const
  {
    throw std::length_error("SplitBuffer length error");
  }

  template < typename T >
  void Buffer< T >::allocate(const size_type size)
  {
    assert(capacity() == 0 && "allocate() called in already allocated SplitBuffer");
    begin_ = static_cast< pointer >(operator new(size * sizeof(T)));
    end_ = begin_;
    last_ = begin_ + size;
  }

  template < typename T >
  void Buffer< T >::deallocate() noexcept
  {
    operator delete(begin_);
    begin_ = nullptr;
    end_ = nullptr;
    last_ = nullptr;
  }

  template < typename T >
  void Buffer< T >::expand(const size_type newCapacity)
  {
    assert(newCapacity > capacity() && "expandStorage(): less or equal capacity");
    Buffer< T > newBuffer;
    newBuffer.allocate(newCapacity);
    newBuffer.constructAtEnd(begin(), end());
    swap(newBuffer);
  }

  template < typename T >
  typename Buffer< T >::size_type Buffer< T >::growthCapacity(
      const size_type newCapacity) const
  {
    const size_type maxSz = maxSize();
    if (newCapacity > maxSz) {
      throw_length_error();
    }
    const size_type cap = capacity();
    if (cap >= maxSz / 2) {
      return maxSz;
    }
    return std::max(cap * 2, newCapacity);
  }

  template < typename T >
  template < typename InputIt, enable_if_input_iterator< InputIt > >
  void Buffer< T >::constructAtEnd(InputIt first, const InputIt last)
  {
    end_ = uninitializedCopy(first, last, end_);
  }

  template < typename T >
  void Buffer< T >::constructAtEnd(const size_type sz, const_reference value)
  {
    assert(sz <= capacity() - size() && "constructAtEnd(): insufficient back spare");
    end_ = uninitializedFill(end_, end_ + sz, value);
  }

  template < typename T >
  void Buffer< T >::destroyAtEnd(pointer newEnd) noexcept
  {
    const bool assertion = newEnd >= begin_ && newEnd <= end_;
    assert(assertion && "destroyAtEnd(): invalid newEnd");
    destroy(newEnd, end_);
    end_ = newEnd;
  }

  template < typename T >
  void Buffer< T >::destroyAtBegin(pointer newBegin) noexcept
  {
    const bool assertion = newBegin >= begin_ && newBegin <= end_;
    assert(assertion && "destroyAtBegin(): invalid newBegin");
    pointer i = destroy(begin_, newBegin);
    moveRange(i, end_, begin_);
    end_ -= newBegin - begin_;
  }

  template < typename T >
  void swap(Buffer< T >& lhs, Buffer< T >& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  template < typename T >
  bool operator==(const Buffer< T >& lhs, const Buffer< T >& rhs)
  {
    return lhs.size() == rhs.size() && compare(lhs.begin(), lhs.end(), rhs.begin());
  }

  template < typename T >
  bool operator!=(const Buffer< T >& lhs, const Buffer< T >& rhs)
  {
    return !(lhs == rhs);
  }

  template < typename T >
  bool operator<(const Buffer< T >& lhs, const Buffer< T >& rhs)
  {
    return lexicographicalCompare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  template < typename T >
  bool operator>(const Buffer< T >& lhs, const Buffer< T >& rhs)
  {
    return rhs < lhs;
  }

  template < typename T >
  bool operator<=(const Buffer< T >& lhs, const Buffer< T >& rhs)
  {
    return !(rhs < lhs);
  }

  template < typename T >
  bool operator>=(const Buffer< T >& lhs, const Buffer< T >& rhs)
  {
    return !(lhs < rhs);
  }
}

#endif

