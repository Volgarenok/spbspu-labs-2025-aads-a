#ifndef HASH_ITERATORS_HPP
#define HASH_ITERATORS_HPP
#include <cassert>
#include <iterator>
#include "hash-node.hpp"

namespace alymova
{
  template< class Key, class Value, class Hash, class KeyEqual >
  class HashTable;

  template< class Key, class Value, class Hash = std::hash< Key >, class KeyEqual = std::equal_to< Key > >
  struct HashConstIterator:
    public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
    using Node = detail::HashNode< Key, Value >;
    using NodeState = typename HashTable< Key, Value, Hash, KeyEqual >::NodeState;
    using T = std::pair< NodeState, Node >;

    HashConstIterator() = default;
    HashConstIterator& operator++() noexcept;
    HashConstIterator operator++(int) noexcept;
    bool operator==(const HashConstIterator& other) const noexcept;
    bool operator!=(const HashConstIterator& other) const noexcept;
    const std::pair< Key, Value >& operator*() const noexcept;
    const std::pair< Key, Value >* operator->() const noexcept;
  protected:
    T* node_;
    T* end_;

    HashConstIterator(T* node, T* end) noexcept;

    friend class HashTable< Key, Value, Hash, KeyEqual >;
  };

  template< class Key, class Value, class Hash = std::hash< Key >, class KeyEqual = std::equal_to< Key > >
  struct HashIterator final:
    public HashConstIterator< Key, Value, Hash, KeyEqual >
  {
    using Base = HashConstIterator< Key, Value, Hash, KeyEqual >;
    using Node = detail::HashNode< Key, Value >;
    using NodeState = typename HashTable< Key, Value, Hash, KeyEqual >::NodeState;
    using T = std::pair< NodeState, Node >;

    std::pair< Key, Value >& operator*() noexcept;
    std::pair< Key, Value >* operator->() noexcept;
  private:
    HashIterator(T* node, T* end) noexcept;

    friend class HashTable< Key, Value, Hash, KeyEqual >;
  };

  template< class Key, class Value, class Hash, class KeyEqual >
  HashConstIterator< Key, Value, Hash, KeyEqual >::HashConstIterator(T* node, T* end) noexcept:
    node_(node),
    end_(end)
  {}

  template< class Key, class Value, class Hash, class KeyEqual >
  HashConstIterator< Key, Value, Hash, KeyEqual >&
    HashConstIterator< Key, Value, Hash, KeyEqual >::operator++() noexcept
  {
    assert(node_ != end_ && "You try to access beyond table's bound");

    node_++;
    if (node_ == end_)
    {
      return *this;
    }
    if (node_->first == NodeState::Fill)
    {
      return *this;
    }
    return ++(*this);
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashConstIterator< Key, Value, Hash, KeyEqual >
    HashConstIterator< Key, Value, Hash, KeyEqual >::operator++(int) noexcept
  {
    HashConstIterator old = *this;
    ++(*this);
    return old;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  bool HashConstIterator< Key, Value, Hash, KeyEqual >::operator==(const HashConstIterator& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  bool HashConstIterator< Key, Value, Hash, KeyEqual >::operator!=(const HashConstIterator& other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  const std::pair< Key, Value >&
    HashConstIterator< Key, Value, Hash, KeyEqual >::operator*() const noexcept
  {
    assert(node_->first != NodeState::Empty && "You try to dereference empty node");

    return node_->second.data;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  const std::pair< Key, Value >*
    HashConstIterator< Key, Value, Hash, KeyEqual >::operator->() const noexcept
  {
    assert(node_->first != NodeState::Empty && "You try to dereference empty node");

    return std::addressof(node_->second.data);
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual >::HashIterator(T* node, T* end) noexcept:
    Base(node, end)
  {}

  template< class Key, class Value, class Hash, class KeyEqual >
  std::pair< Key, Value >& HashIterator< Key, Value, Hash, KeyEqual >::operator*() noexcept
  {
    assert(Base::node_->first != NodeState::Empty && "You try to dereference empty node");

    return Base::node_->second.data;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  std::pair< Key, Value >* HashIterator< Key, Value, Hash, KeyEqual >::operator->() noexcept
  {
    assert(Base::node_->first != NodeState::Empty && "You try to dereference empty node");

    return std::addressof(Base::node_->second.data);
  }
}

#endif
