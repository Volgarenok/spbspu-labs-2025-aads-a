#ifndef HASHITERATOR_HPP
#define HASHITERATOR_HPP

#include <iterator>

namespace kushekbaev
{
  template< typename Key, typename Value, typename Hash, typename Equal >
  struct HashTable;

  template< typename Key, typename Value, typename Hash = std::hash< Key >, typename Equal = std::equal_to< Key > >
  struct HashIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    HashIterator();
    HashIterator(const HashIterator& other);
    HashIterator(HashIterator&& other);
    ~HashIterator() = default;
    HashIterator& operator=(const HashIterator& other) = default;
    HashIterator& operator++();
    HashIterator operator++(int);
    HashIterator& operator--();
    HashIterator operator--(int);
    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();
    bool operator==(const HashIterator& other) const;
    bool operator!=(const HashIterator& other) const;

    private:
      friend class HashTable< Key, Value, Hash, Equal >;
      HashTable< Key, Value, Hash, Equal >* table_;
      size_t index_;
      HashIterator(HashTable< Key, Value, Hash, Equal >* table, size_t index);
      void skip_empty_slots();
  };

  template< typename Key, typename Value, typename Hash, typename Equal >
  void HashIterator< Key, Value, Hash, Equal >::skip_empty_slots()
  {
    while (index_ < table_->table_.size() && (!table_->table_[index_].occupied || table_->table_[index_].deleted))
    {
      ++index_;
    }
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >::HashIterator():
    table_(nullptr),
    index_(0)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >::HashIterator(const HashIterator& other):
    table_(other.table_),
    index_(other.index_)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >::HashIterator(HashIterator&& other):
    table_(std::move(other.table_)),
    index_(std::move(other.index_))
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >& HashIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    skip_empty_slots();
    return *this;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal > HashIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    HashIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >& HashIterator< Key, Value, Hash, Equal >::operator--()
  {
    --index_;
    skip_empty_slots();
    return *this;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal > HashIterator< Key, Value, Hash, Equal >::operator--(int)
  {
    HashIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  std::pair< Key, Value >& HashIterator< Key, Value, Hash, Equal >::operator*()
  {
    return table_->table_[index_].data_;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  std::pair< Key, Value >* HashIterator< Key, Value, Hash, Equal >::operator->()
  {
    return std::addressof(table_->table_[index_].data_);
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  bool HashIterator< Key, Value, Hash, Equal >::operator==(const HashIterator& other) const
  {
    return table_ == other.table_ && index_ == other.index_;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  bool HashIterator< Key, Value, Hash, Equal >::operator!=(const HashIterator& other) const
  {
    return !(*this == other);
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >::HashIterator(HashTable< Key, Value, Hash, Equal >* table, size_t index):
    table_(table),
    index_(index)
  {
    skip_empty_slots();
  }
}

#endif
