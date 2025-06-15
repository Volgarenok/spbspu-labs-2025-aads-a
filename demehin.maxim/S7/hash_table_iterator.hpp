#ifndef HASH_TABLE_ITERATOR
#define HASH_TABLE_ITERATOR
#include <type_traits>
#include <cstddef>
#include <utility>

namespace demehin
{
  template< typename Key, typename T, typename Hash, typename Equal >
  class HashTable;

  template< typename Key, typename T, typename Hash, typename Equal, bool isConst >
  class HashTIterator
  {
    friend class HashTable< Key, T, Hash, Equal >;
    friend class HashTIterator< Key, T, Hash, Equal, true >;
    friend class HashTIterator< Key, T, Hash, Equal, false >;
  public:
    using this_t = HashTIterator< Key, T, Hash, Equal, isConst >;
    using Val = typename std::conditional< isConst, const std::pair< Key, T >, std::pair< Key, T > >::type;
    using Ref = typename std::conditional< isConst, const std::pair< Key, T >&, std::pair< Key, T >& >::type;
    using Ptr = typename std::conditional< isConst, const std::pair< Key, T >*, std::pair< Key, T >* >::type;
    using Table = typename std::conditional< isConst, const HashTable< Key, T, Hash, Equal >, HashTable< Key, T, Hash, Equal > >::type;

    HashTIterator() noexcept:
      table_(nullptr),
      index_(0)
    {}

    template< bool OtherConst, typename = std::enable_if_t< isConst || !OtherConst > >
    HashTIterator(const HashTIterator< Key, T, Hash, Equal, OtherConst >& other) noexcept:
      table_(other.table_),
      index_(other.index_)
    {}

    this_t& operator++() noexcept
    {
      ++index_;
      skipEmpty();
      return *this;
    }

    this_t operator++(int) noexcept
    {
      this_t tmp = *this;
      ++(*this);
      return tmp;
    }

    Ref operator*() const noexcept
    {
      return table_->slots_[index_].pair;
    }

    Ptr operator->() const noexcept
    {
      return std::addressof(table_->slots_[index_].pair);
    }

    bool operator==(const this_t& rhs) const noexcept
    {
      return table_ == rhs.table_ && index_ == rhs.index_;
    }

    bool operator!=(const this_t& rhs) const noexcept
    {
      return !(*this == rhs);
    }

  private:
    Table* table_;
    size_t index_;

    HashTIterator(Table* table, size_t index) noexcept:
      table_(table),
      index_(index)
    {
      skipEmpty();
    }

    void skipEmpty() noexcept
    {
      while (index_ < table_->slots_.size() && table_->slots_[index_].state != HashTable< Key, T, Hash, Equal >::SlotState::OCCUPIED)
      {
        ++index_;
      }
    }
  };
}

#endif
