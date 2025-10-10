#ifndef CONSTHASHITERATOR_HPP
#define CONSTHASHITERATOR_HPP

#include <iterator>

namespace kushekbaev
{
  template< typename Key, typename Value, typename Hash, typename Equal >
  struct HashTable;

  template< typename Key, typename Value, typename Hash = std::hash< Key >, typename Equal = std::equal_to< Key > >
  struct ConstHashIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    ConstHashIterator();
    ConstHashIterator(const ConstHashIterator& other);
    ConstHashIterator(ConstHashIterator&& other);
    ~ConstHashIterator() = default;
    ConstHashIterator& operator=(const ConstHashIterator& other) = default;
    ConstHashIterator& operator++();
    ConstHashIterator operator++(int);
    ConstHashIterator& operator--();
    ConstHashIterator operator--(int);
    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;
    bool operator==(const ConstHashIterator& other) const;
    bool operator!=(const ConstHashIterator& other) const;

    private:
      friend class HashTable< Key, Value, Hash, Equal >;
      const HashTable< Key, Value, Hash, Equal >* table_;
      size_t index_;
      ConstHashIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index);
      void skip_empty_slots();
  };

  template< typename Key, typename Value, typename Hash, typename Equal >
  void ConstHashIterator< Key, Value, Hash, Equal >::skip_empty_slots()
  {
    while (index_ < table_->table_.size() && (!table_->table_[index_].occupied || table_->table_[index_].deleted))
    {
      ++index_;
    }
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal >::ConstHashIterator():
    table_(nullptr),
    index_(0)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal >::ConstHashIterator(const ConstHashIterator& other):
    table_(other.table_),
    index_(other.index_)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal >::ConstHashIterator(ConstHashIterator&& other):
    table_(std::move(other.table_)),
    index_(std::move(other.index_))
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal >& ConstHashIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    skip_empty_slots();
    return *this;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal > ConstHashIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    ConstHashIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal >& ConstHashIterator< Key, Value, Hash, Equal >::operator--()
  {
    --index_;
    skip_empty_slots();
    return *this;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal > ConstHashIterator< Key, Value, Hash, Equal >::operator--(int)
  {
    ConstHashIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  const std::pair< Key, Value >& ConstHashIterator< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->table_[index_].data_;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  const std::pair< Key, Value >* ConstHashIterator< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(table_->table_[index_].data_);
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  bool ConstHashIterator< Key, Value, Hash, Equal >::operator==(const ConstHashIterator& other) const
  {
    return table_ == other.table_ && index_ == other.index_;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  bool ConstHashIterator< Key, Value, Hash, Equal >::operator!=(const ConstHashIterator& other) const
  {
    return !(*this == other);
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal >::ConstHashIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index):
    table_(table),
    index_(index)
  {
    skip_empty_slots();
  }
}

#endif
