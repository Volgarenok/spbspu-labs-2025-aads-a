#ifndef ITERATOR_HPP
#define ITERATOR_HPP

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
  class ConstIterator;

  template< class Key, class T, class Hash = boost::hash< Key >>
  class Iterator
  {
    friend class ConstIterator< Key, T, Hash >;
    friend class RobinHoodHashTable< Key, T, Hash >;
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair< const Key, T >;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    Iterator() noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    Iterator& operator++() noexcept;
    Iterator operator++(int) noexcept;
    bool operator==(const Iterator&) const noexcept;
    bool operator!=(const Iterator&) const noexcept;

  private:
    Entry< Key, T >* current_;
    Entry< Key, T >* end_;
    explicit Iterator(Entry< Key, T >*, Entry< Key, T >*) noexcept;
  };

  template< class Key, class T, class Hash >
  Iterator< Key, T, Hash >::Iterator() noexcept:
    current_(nullptr),
    end_(nullptr)
  {}

  template< class Key, class T, class Hash >
  Iterator< Key, T, Hash >::Iterator(Entry< Key, T >* current, Entry< Key, T >* end) noexcept:
    current_(current),
    end_(end)
  {}

  template< class Key, class T, class Hash >
  typename Iterator< Key, T, Hash >::reference Iterator< Key, T, Hash >::operator*() const noexcept
  {
    return reinterpret_cast< reference >(current_->data);
  }

  template< class Key, class T, class Hash >
  typename Iterator< Key, T, Hash >::pointer Iterator< Key, T, Hash >::operator->() const noexcept
  {
    return reinterpret_cast< pointer >(&current_->data);
  }

  template< class Key, class T, class Hash >
  Iterator< Key, T, Hash >& Iterator< Key, T, Hash >::operator++() noexcept
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
  Iterator< Key, T, Hash >Iterator< Key, T, Hash >::operator++(int) noexcept
  {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class T, class Hash >
  bool Iterator< Key, T, Hash >::operator==(const Iterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template< class Key, class T, class Hash >
  bool Iterator< Key, T, Hash >::operator!=(const Iterator& other) const noexcept
  {
    return !(*this == other);
  }

}
#endif
