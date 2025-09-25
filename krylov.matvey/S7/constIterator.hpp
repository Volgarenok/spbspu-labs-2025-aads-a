#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP
#include <iterator>
#include "node.hpp"

namespace krylov
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value, class Hash, class Equal >
  struct ConstIterator: std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
    using cHash = const HashTable< Key, Value, Hash, Equal >;
    using cIter = const ConstIterator< Key, Value, Hash, Equal >;
    ConstIterator();
    ConstIterator(const HashTable< Key, Value, Hash, Equal >* t, size_t i, const Node< Key, Value >* n);
    ConstIterator(const ConstIterator< Key, Value, Hash, Equal >& c_iter) = default;
    ~ConstIterator() = default;
    ConstIterator< Key, Value, Hash, Equal >& operator=(const ConstIterator< Key, Value, Hash, Equal >& c_iter) = default;
    ConstIterator< Key, Value, Hash, Equal >& operator++() noexcept;
    ConstIterator< Key, Value, Hash, Equal > operator++(int) noexcept;
    bool operator==(const ConstIterator< Key, Value, Hash, Equal >& c_iter) const noexcept;
    bool operator!=(const ConstIterator< Key, Value, Hash, Equal >& c_iter) const noexcept;
    const std::pair< Key, Value >& operator*() const noexcept;
    const std::pair< Key, Value >* operator->() const noexcept;
  private:
    const HashTable< Key, Value, Hash, Equal >* table_;
    size_t ind_;
    const Node< Key, Value >* node_;

    void goNext() noexcept;
  };

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal >::ConstIterator():
     table_(nullptr),
    ind_(0),
    node_(nullptr)
  {}

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal >::ConstIterator(cHash* t, size_t i, const Node< Key, Value >* n):
    table_(t),
    ind_(i),
    node_(n)
  {}

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal >& ConstIterator< Key, Value, Hash, Equal >::operator++() noexcept
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
  void ConstIterator< Key, Value, Hash, Equal >::goNext() noexcept
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
  ConstIterator< Key, Value, Hash, Equal > ConstIterator< Key, Value, Hash, Equal >::operator++(int) noexcept
  {
    ConstIterator< Key, Value, Hash, Equal > tmp(*this);
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool ConstIterator< Key, Value, Hash, Equal >::operator==(cIter& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool ConstIterator< Key, Value, Hash, Equal >::operator!=(cIter& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value >& ConstIterator< Key, Value, Hash, Equal >::operator*() const noexcept
  {
    return node_->data_;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value >* ConstIterator< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    return std::addressof(node_->data_);
  }
}

#endif
