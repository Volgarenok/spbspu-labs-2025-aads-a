#ifndef HASH_СITER__HPP
#define HASH_СITER__HPP
#include <iterator>

namespace tkach
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashIterator;

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class CHashIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashIterator< Key, Value, Hash, Equal >;
  public:
    using this_t = CHashIterator< Key, Value, Hash, Equal >;
    using Table =  HashTable< Key, Value, Hash, Equal >;
    CHashIterator();
    CHashIterator(const this_t&) = default;
    ~CHashIterator() = default;
    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    std::pair< Key, Value > & operator*() const;
    std::pair< Key, Value > * operator->() const;
    bool operator!=(const this_t& rhs) const;
    bool operator==(const this_t& rhs) const;
  private:
    Table* ptr_table_;
    size_t pos_;
    explicit CHashIterator(const Table* table, size_t pos);
    explicit CHashIterator(HashIterator<Key, Value, Hash, Equal> other);
    void advance_to_prev_occupied();
    void advance_to_next_occupied();
  };

  template<class Key, class Value, class Hash, class Equal>
  CHashIterator<Key, Value, Hash, Equal>::CHashIterator(HashIterator<Key, Value, Hash, Equal> other):
      ptr_table_(other.ptr_table_),
      pos_(other.pos_)
  {}

  template< class Key, class Value, class Hash, class Equal >
  void CHashIterator< Key, Value, Hash, Equal >::advance_to_prev_occupied()
  {
    while (ptr_table_ && pos_ > 0 && ptr_table_->table_[pos_].state != Table::EntryState::Occupied)
    {
      pos_--;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void CHashIterator< Key, Value, Hash, Equal >::advance_to_next_occupied()
  {
    while (ptr_table_ && pos_ < ptr_table_->table_.size() && ptr_table_->table_[pos_].state != Table::EntryState::Occupied)
    {
      pos_++;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  CHashIterator< Key, Value, Hash, Equal >::CHashIterator():
    ptr_table_(nullptr),
    pos_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  CHashIterator< Key, Value, Hash, Equal >::CHashIterator(const Table* table, size_t pos):
    ptr_table_(const_cast< Table* >(table)),
    pos_(pos)
  {
    advance_to_next_occupied();
  }

  template< class Key, class Value, class Hash, class Equal >
  CHashIterator< Key, Value, Hash, Equal >& CHashIterator< Key, Value, Hash, Equal >::operator++()
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
  CHashIterator< Key, Value, Hash, Equal > CHashIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class Value, class Hash, class Equal >
  CHashIterator< Key, Value, Hash, Equal >& CHashIterator< Key, Value, Hash, Equal >::operator--()
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
  CHashIterator< Key, Value, Hash, Equal > CHashIterator< Key, Value, Hash, Equal >::operator--(int)
  {
    this_t result(*this);
    --(*this);
    return result;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value >& CHashIterator< Key, Value, Hash, Equal >::operator*() const
  {
    return ptr_table_->table_[pos_].pair;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value >* CHashIterator< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(ptr_table_->table_[pos_].pair);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool CHashIterator< Key, Value, Hash, Equal >::operator==(const this_t& rhs) const
  {
    return ptr_table_ == rhs.ptr_table_ && pos_ == rhs.pos_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool CHashIterator< Key, Value, Hash, Equal >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
