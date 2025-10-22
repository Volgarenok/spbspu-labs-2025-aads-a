#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include "hashTable.hpp"
#include "constIterator.hpp"

namespace krylov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class ConstIterator;

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class Iterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class ConstIterator< Key, Value, Hash, Equal >;
  public:
    Iterator();
    Iterator(const Iterator< Key, Value, Hash, Equal >&) = default;
    Iterator< Key, Value, Hash, Equal >& operator=(const Iterator< Key, Value, Hash, Equal >&) = default;
    ~Iterator() = default;
    Iterator< Key, Value, Hash, Equal >& operator++();
    Iterator< Key, Value, Hash, Equal > operator++(int);
    Iterator< Key, Value, Hash, Equal >& operator--();
    Iterator< Key, Value, Hash, Equal > operator--(int);
    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();
    bool operator==(const Iterator< Key, Value, Hash, Equal >& rhs) const;
    bool operator!=(const Iterator< Key, Value, Hash, Equal >& rhs) const;
  private:
    HashTable< Key, Value, Hash, Equal >* table_;
    size_t index_;
    Iterator(HashTable< Key, Value, Hash, Equal >* table, size_t index);
    void skipEmpty();
  };

  template< class Key, class Value, class Hash, class Equal >
  void Iterator< Key, Value, Hash, Equal >::skipEmpty()
  {
    while (index_ < table_->table_.size() && (!table_->table_[index_].occupied || table_->table_[index_].deleted))
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal >::Iterator():
    table_(nullptr),
    index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal >::Iterator(HashTable< Key, Value, Hash, Equal >* table, size_t index):
    table_(table),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal >& Iterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal > Iterator< Key, Value, Hash, Equal >::operator++(int)
  {
    Iterator< Key, Value, Hash, Equal > tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal >& Iterator< Key, Value, Hash, Equal >::operator--()
  {
    --index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal > Iterator< Key, Value, Hash, Equal >::operator--(int)
  {
    Iterator< Key, Value, Hash, Equal > tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value >& Iterator< Key, Value, Hash, Equal >::operator*()
  {
    return table_->table_[index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value >* Iterator< Key, Value, Hash, Equal >::operator->()
  {
    return std::addressof(table_->table_[index_].data);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool Iterator< Key, Value, Hash, Equal >::operator==(const Iterator< Key, Value, Hash, Equal > & rhs) const
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool Iterator< Key, Value, Hash, Equal >::operator!=(const Iterator< Key, Value, Hash, Equal > & rhs) const
  {
    return !(*this == rhs);
  }
}

#endif
