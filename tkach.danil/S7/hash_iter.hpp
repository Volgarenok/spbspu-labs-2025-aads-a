#ifndef HASH_ITER_HPP
#define HASH_ITER_HPP
#include <iterator>

namespace tkach
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class CHashIterator;

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class CHashIterator< Key, Value, Hash, Equal >;
  public:
    using this_t = HashIterator< Key, Value, Hash, Equal >;
    using Table =  HashTable< Key, Value, Hash, Equal >;
    HashIterator();
    HashIterator(const this_t&) = default;
    ~HashIterator() = default;
    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    std::pair< Key, Value > & operator*();
    std::pair< Key, Value > * operator->();
    bool operator!=(const this_t& rhs) const;
    bool operator==(const this_t& rhs) const;
  private:
    Table* ptr_table_;
    size_t pos_;
    explicit HashIterator(Table* table, size_t pos);
    explicit HashIterator(CHashIterator<Key, Value, Hash, Equal> other);
    void advance_to_prev_occupied();
    void advance_to_next_occupied();
  };

  template<class Key, class Value, class Hash, class Equal>
  HashIterator<Key, Value, Hash, Equal>::HashIterator(CHashIterator<Key, Value, Hash, Equal> other):
      ptr_table_(other.ptr_table_),
      pos_(other.pos_)
  {}

  template< class Key, class Value, class Hash, class Equal >
  void HashIterator< Key, Value, Hash, Equal >::advance_to_prev_occupied()
  {
    while (ptr_table_ && pos_ > 0 && ptr_table_->table_[pos_].state != Table::EntryState::Occupied)
    {
      pos_--;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashIterator< Key, Value, Hash, Equal >::advance_to_next_occupied()
  {
    while (ptr_table_ && pos_ < ptr_table_->table_.size() && ptr_table_->table_[pos_].state != Table::EntryState::Occupied)
    {
      pos_++;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal >::HashIterator():
    ptr_table_(nullptr),
    pos_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal >::HashIterator(Table* table, size_t pos):
    ptr_table_(table),
    pos_(pos)
  {
    advance_to_next_occupied();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal >& HashIterator< Key, Value, Hash, Equal >::operator++()
  {
    if (pos_ >= ptr_table_->table_.size())
    {
      return *this;
    }
    ++pos_;
    advance_to_next_occupied();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal >& HashIterator< Key, Value, Hash, Equal >::operator--()
  {
    if (pos_ <= 0)
    {
      return *this;
    }
    --pos_;
    advance_to_prev_occupied();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashIterator< Key, Value, Hash, Equal >::operator--(int)
  {
    this_t result(*this);
    --(*this);
    return result;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value >& HashIterator< Key, Value, Hash, Equal >::operator*()
  {
    return ptr_table_->table_[pos_].pair;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value >* HashIterator< Key, Value, Hash, Equal >::operator->()
  {
    return std::addressof(ptr_table_->table_[pos_].pair);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashIterator< Key, Value, Hash, Equal >::operator==(const this_t& rhs) const
  {
    return ptr_table_ == rhs.ptr_table_ && pos_ == rhs.pos_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashIterator< Key, Value, Hash, Equal >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
