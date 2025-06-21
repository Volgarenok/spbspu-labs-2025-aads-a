#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <boost/functional/hash.hpp>

#include "entryState.hpp"

namespace dribas
{
  template< class Key, class T, class Hash >
  class RobinHoodHashTable;

  template< class Key, class T >
  struct Entry;

  template< class Key, class T, class Hash >
  class Iterator;

  template< class Key, class T, class Hash = boost::hash< Key >>
  class ConstIterator
  {
    friend class RobinHoodHashTable< Key, T, Hash >;
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const std::pair< const Key, T >;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    ConstIterator() noexcept;
    ConstIterator(const Iterator< Key, T, Hash >&) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    ConstIterator& operator++() noexcept;
    ConstIterator operator++(int) noexcept;
    bool operator==(const ConstIterator&) const noexcept;
    bool operator!=(const ConstIterator&) const noexcept;

  private:
    const Entry< Key, T >* current_;
    const Entry< Key, T >* end_;
    explicit ConstIterator(const Entry< Key, T >*, const Entry< Key, T >*) noexcept;
  };

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >::ConstIterator() noexcept:
    current_(nullptr),
    end_(nullptr)
  {}

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >::ConstIterator(const Iterator< Key, T, Hash >& it) noexcept:
    current_(it.current_),
    end_(it.end_)
  {}

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >::ConstIterator(const Entry< Key, T >* current, const Entry< Key, T >* end) noexcept:
    current_(current),
    end_(end)
  {}

  template< class Key, class T, class Hash >
  typename ConstIterator< Key, T, Hash >::reference ConstIterator< Key, T, Hash >::operator*() const noexcept
  {
    return reinterpret_cast< reference >(current_->data);
  }

  template< class Key, class T, class Hash >
  typename ConstIterator< Key, T, Hash >::pointer ConstIterator< Key, T, Hash >::operator->() const noexcept
  {
    return reinterpret_cast< pointer >(&current_->data);
  }

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >& ConstIterator< Key, T, Hash >::operator++() noexcept
  {
    if (current_ == end_) {
      return *this;
    }

    do {
      ++current_;
    } while (current_ != end_ && current_->state != EntryState::OCCUPIED);
    return *this;
  }

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash > ConstIterator< Key, T, Hash >::operator++(int) noexcept
  {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class T, class Hash >
  bool ConstIterator< Key, T, Hash >::operator==(const ConstIterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template< class Key, class T, class Hash >
  bool ConstIterator< Key, T, Hash >::operator!=(const ConstIterator& other) const noexcept
  {
    return !(*this == other);
  }
}

#endif
