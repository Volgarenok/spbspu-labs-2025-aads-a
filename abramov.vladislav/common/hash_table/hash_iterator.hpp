#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include "decls.hpp"
#include "hash_node.hpp"

namespace abramov
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value, class Hash, class Equal >
  struct HashIterator: std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
    using HashT = HashTable< Key, Value, Hash, Equal >;
    using Iter = const HashIterator< Key, Value, Hash, Equal >;

    HashIterator();
    HashIterator(HashTable< Key, Value, Hash, Equal > *t, size_t i, HashNode< Key, Value > *n);
    HashIterator(const HashIterator< Key, Value, Hash, Equal > &iter) = default;
    ~HashIterator() = default;
    HashIterator< Key, Value, Hash, Equal > &operator=(const HashIterator< Key, Value, Hash, Equal > &iter) = default;
    HashIterator< Key, Value, Hash, Equal > &operator++() noexcept;
    HashIterator< Key, Value, Hash, Equal > operator++(int) noexcept;
    bool operator==(const HashIterator< Key, Value, Hash, Equal > &iter) const noexcept;
    bool operator!=(const HashIterator< Key, Value, Hash, Equal > &iter) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;
    std::pair< Key, Value > *operator->() noexcept;
  private:
    HashTable< Key, Value, Hash, Equal > *table_;
    size_t ind_;
    HashNode< Key, Value > *node_;

    void goNext() noexcept;
  };
}

template< class Key, class Value, class Hash, class Equal >
abramov::HashIterator< Key, Value, Hash, Equal >::HashIterator():
  table_(nullptr),
  ind_(0),
  node_(nullptr)
{}

template< class Key, class Value, class Hash, class Equal >
abramov::HashIterator< Key, Value, Hash, Equal >::HashIterator(HashT *t, size_t i, HashNode< Key, Value > *n):
  table_(t),
  ind_(i),
  node_(n)
{}

template< class Key, class Value, class Hash, class Equal >
abramov::HashIterator< Key, Value, Hash, Equal > &abramov::HashIterator< Key, Value, Hash, Equal >::operator++() noexcept
{
  if (node_)
  {
    node_ = node_->next_;
    if (!node_)
    {
      goNext();
    }
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void abramov::HashIterator< Key, Value, Hash, Equal >::goNext() noexcept
{
  ++ind_;
  while (ind_ < table_->capacity_ && !table_->table_[ind_])
  {
    ++ind_;
  }
  if (ind_ < table_->capacity_)
  {
    node_ = table_->table_[ind_];
  }
  else
  {
    node_ = nullptr;
  }
}

template< class Key, class Value, class Hash, class Equal >
abramov::HashIterator< Key, Value, Hash, Equal > abramov::HashIterator< Key, Value, Hash, Equal >::operator++(int) noexcept
{
  HashIterator< Key, Value, Hash, Equal > tmp(*this);
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
bool abramov::HashIterator< Key, Value, Hash, Equal >::operator==(Iter &rhs) const noexcept
{
  return node_ == rhs.node_;
}

template< class Key, class Value, class Hash, class Equal >
bool abramov::HashIterator< Key, Value, Hash, Equal >::operator!=(Iter &rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > &abramov::HashIterator< Key, Value, Hash, Equal >::operator*() noexcept
{
  return node_->data_;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > *abramov::HashIterator< Key, Value, Hash, Equal >::operator->() noexcept
{
  return std::addressof(node_->data_);
}
#endif
