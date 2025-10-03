#ifndef HASHTABLEITERATOR_HPP
#define HASHTABLEITERATOR_HPP
#include <iterator>

namespace finaev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator;

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTableIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashTableConstIterator< Key, Value, Hash, Equal >;
  public:
    using this_t = HashTableIterator< Key, Value, Hash, Equal >;

    HashTableIterator();
    HashTableIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;
    ~HashTableIterator() = default;
    this_t & operator++();
    this_t operator++(int);
    this_t & operator--();
    this_t operator--(int);
    std::pair< Key, Value > & operator*();
    std::pair< Key, Value > * operator->();
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  private:
    HashTable< Key, Value, Hash, Equal >* table_;
    size_t index_;
    HashTableIterator(HashTable< Key, Value, Hash, Equal >* table, size_t index);
    void skipEmpty();
  };

  template< class Key, class Value, class Hash, class Equal >
  void HashTableIterator< Key, Value, Hash, Equal >::skipEmpty()
  {
    while (index_ < table_->table_.size() && (!table_->table_[index_].occupied || table_->table_[index_].deleted))
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator():
    table_(nullptr),
    index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator(HashTable< Key, Value, Hash, Equal >* table, size_t index):
    table_(table),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >& HashTableIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal > HashTableIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    this_t tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >& HashTableIterator< Key, Value, Hash, Equal >::operator--()
  {
    --index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal > HashTableIterator< Key, Value, Hash, Equal >::operator--(int)
  {
    this_t tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value >& HashTableIterator< Key, Value, Hash, Equal >::operator*()
  {
    return table_->table_[index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value >* HashTableIterator< Key, Value, Hash, Equal >::operator->()
  {
    return std::addressof(table_->table_[index_].data);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableIterator< Key, Value, Hash, Equal >::operator==(const this_t & rhs) const
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableIterator< Key, Value, Hash, Equal >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }
}

#endif
