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
    using this_t = ConstHashIterator< Key, Value, Hash, Equal >;

    ConstHashIterator();
    ConstHashIterator(const this_t& other) = default;
    ~ConstHashIterator() = default;
    this_t& operator=(const this_t& other) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;
    bool operator==(const this_t& other) const;
    bool operator!=(const this_t& other) const;

    private:
      friend class HashTable< Key, Value, Hash, Equal >;
      const HashTable< Key, Value, Hash, Equal >* table_;
      size_t index_;
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
  using this_t = ConstHashIterator< Key, Value, Hash, Equal >;

  template< typename Key, typename Value, typename Hash, typename Equal >
  ConstHashIterator< Key, Value, Hash, Equal >::ConstHashIterator():
    table_(nullptr),
    index_(0)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  this_t< Key, Value, Hash, Equal >& ConstHashIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    skip_empty_slots();
    return *this;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  this_t< Key, Value, Hash, Equal > ConstHashIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    this_t tmp = *this;
    ++(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  this_t< Key, Value, Hash, Equal >& ConstHashIterator< Key, Value, Hash, Equal >::operator++()
  {
    --index_;
    skip_empty_slots();
    return *this;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  this_t< Key, Value, Hash, Equal > ConstHashIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    this_t tmp = *this;
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
  bool ConstHashIterator< Key, Value, Hash, Equal >::operator==(const this_t& other) const
  {
    return table_ == other.table_ && index_ == other.index_;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  bool ConstHashIterator< Key, Value, Hash, Equal >::operator==(const this_t& other) const
  {
    return !(*this == rhs)
  }
}

#endif
