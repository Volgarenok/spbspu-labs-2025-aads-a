#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP
#include <iterator>
#include "hashTable.hpp"
#include "iterator.hpp"

namespace krylov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class Iterator;

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class ConstIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class Iterator< Key, Value, Hash, Equal >;
  public:
    ConstIterator();
    ConstIterator(const ConstIterator< Key, Value, Hash, Equal > &) = default;
    ConstIterator< Key, Value, Hash, Equal > & operator=(const ConstIterator< Key, Value, Hash, Equal > &) = default;
    ~ConstIterator() = default;
    ConstIterator< Key, Value, Hash, Equal > & operator++();
    ConstIterator< Key, Value, Hash, Equal > operator++(int);
    ConstIterator< Key, Value, Hash, Equal > & operator--();
    ConstIterator< Key, Value, Hash, Equal > operator--(int);
    const std::pair< Key, Value > & operator*() const;
    const std::pair< Key, Value > * operator->() const;
    bool operator==(const ConstIterator< Key, Value, Hash, Equal > & rhs) const;
    bool operator!=(const ConstIterator< Key, Value, Hash, Equal > & rhs) const;
  private:
    const HashTable< Key, Value, Hash, Equal >* table_;
    size_t index_;
    ConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index);
    void skipEmpty();
  };

  template< class Key, class Value, class Hash, class Equal >
  void ConstIterator< Key, Value, Hash, Equal >::skipEmpty()
  {
    while (index_ < table_->table_.size() && (!table_->table_[index_].occupied || table_->table_[index_].deleted))
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal >::ConstIterator():
    table_(nullptr),
    index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal >::ConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index):
    table_(table),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal >& ConstIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal > ConstIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    ConstIterator< Key, Value, Hash, Equal > tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal >& ConstIterator< Key, Value, Hash, Equal >::operator--()
  {
    --index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal > ConstIterator< Key, Value, Hash, Equal >::operator--(int)
  {
    ConstIterator< Key, Value, Hash, Equal > tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value >& ConstIterator< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->table_[index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value >* ConstIterator< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(table_->table_[index_].data);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool ConstIterator< Key, Value, Hash, Equal >::operator==(const ConstIterator< Key, Value, Hash, Equal > & rhs) const
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool ConstIterator< Key, Value, Hash, Equal >::operator!=(const ConstIterator< Key, Value, Hash, Equal > & rhs) const
  {
    return !(*this == rhs);
  }
}

#endif
