#ifndef HASHCONSTITER_HPP
#define HASHCONSTITER_HPP

#include <iterator>

namespace mozhegova
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashIter;

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashConstIter: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashIter< Key, Value, Hash, Equal >;
  public:
    using this_t = HashConstIter< Key, Value, Hash, Equal >;
    using table =  HashTable< Key, Value, Hash, Equal >;

    HashConstIter();
    HashConstIter(const this_t &) = default;
    ~HashConstIter() = default;
    this_t & operator=(const this_t &) = default;

    this_t & operator++();
    this_t operator++(int);
    this_t & operator--();
    this_t operator--(int);

    const std::pair< Key, Value > & operator*() const;
    const std::pair< Key, Value > * operator->() const;

    bool operator!=(const this_t & rhs) const;
    bool operator==(const this_t & rhs) const;
  private:
    table * table_;
    size_t index_;
    HashConstIter(const table * table, size_t index);
    void skipEmpty();
  };

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal >::HashConstIter():
    table_(nullptr),
    index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal >::HashConstIter(const table * table, size_t index):
    table_(const_cast< HashTable< Key, Value, Hash, Equal > * >(table)),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashConstIter< Key, Value, Hash, Equal >::skipEmpty()
  {
    while (index_ < table_->table_.size() && (!table_->table_[index_].occupied || table_->table_[index_].deleted))
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > & HashConstIter< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > HashConstIter< Key, Value, Hash, Equal >::operator++(int)
  {
    this_t tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > & HashConstIter< Key, Value, Hash, Equal >::operator--()
  {
    --index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > HashConstIter< Key, Value, Hash, Equal >::operator--(int)
  {
    this_t tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value > & HashConstIter< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->table_[index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value > * HashConstIter< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(table_->table_[index_].data);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashConstIter< Key, Value, Hash, Equal >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashConstIter< Key, Value, Hash, Equal >::operator==(const this_t & rhs) const
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }
}

#endif
