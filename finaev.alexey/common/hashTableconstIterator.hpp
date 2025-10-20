#ifndef HASHTABLECONSTITERATOR_hpp
#define HASHTABLECONSTITERATOR_hpp
#include <iterator>

namespace finaev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator;

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTableConstIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashTableIterator< Key, Value, Hash, Equal >;
  public:
    using this_t = HashTableConstIterator< Key, Value, Hash, Equal >;

    HashTableConstIterator();
    HashTableConstIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;
    ~HashTableConstIterator() = default;
    this_t & operator++();
    this_t operator++(int);
    this_t & operator--();
    this_t operator--(int);
    const std::pair< Key, Value > & operator*() const;
    const std::pair< Key, Value > * operator->() const;
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  private:
    const HashTable< Key, Value, Hash, Equal >* table_;
    size_t index_;
    HashTableConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index);
    void skipEmpty();
  };

  template< class Key, class Value, class Hash, class Equal >
  void HashTableConstIterator< Key, Value, Hash, Equal >::skipEmpty()
  {
    while (index_ < table_->table_.size() && (!table_->table_[index_].occupied || table_->table_[index_].deleted))
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator():
    table_(nullptr),
    index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index):
    table_(table),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >& HashTableConstIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal > HashTableConstIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    this_t tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >& HashTableConstIterator< Key, Value, Hash, Equal >::operator--()
  {
    --index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal > HashTableConstIterator< Key, Value, Hash, Equal >::operator--(int)
  {
    this_t tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value >& HashTableConstIterator< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->table_[index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value >* HashTableConstIterator< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(table_->table_[index_].data);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableConstIterator< Key, Value, Hash, Equal >::operator==(const this_t & rhs) const
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableConstIterator< Key, Value, Hash, Equal >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }
}

#endif
