#ifndef ITERATORHASH_HPP
#define ITERATORHASH_HPP
#include <cstddef>
#include <memory>
#include <utility>
namespace brevnov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal, bool IsConst >
  class IteratorHash
  {
  public:
    using value = std::pair< Key, Value >;
    using ref = std::conditional_t< IsConst, const value&, value& >;
    using point = std::conditional_t< IsConst, const value*, value* >;
    using iterator = IteratorHash< Key, Value, Hash, Equal, IsConst >;
    using Table = HashTable< Key, Value, Hash, Equal >;
    using ConstTable = const HashTable< Key, Value, Hash, Equal >;
    using TablePtr = std::conditional_t< IsConst, ConstTable*, Table* >;

    IteratorHash():
      table_(nullptr),
      index_(0)
    {}

    IteratorHash(TablePtr table, size_t index) noexcept:
      table_(table),
      index_(index)
    {}

    template < bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    IteratorHash(const IteratorHash< Key, Value, Hash, Equal, OtherIsConst >& copy) noexcept:
      table_(copy.table_),
      index_(copy.index_)
    {}

    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    iterator& operator=(const IteratorHash< Key, Value, Hash, Equal, OtherIsConst >& copy) noexcept
    {
      table_ = copy.table_;
      index_ = copy.index_;
      return *this;
    }

    iterator& operator++() noexcept
    {
      ++index_;
      while (index_ < table_->slots_.size() && table_->slots_[index_].status != Table::Status::OCCUPIED)
      {
        ++index_;
      }
      return *this;
    }

    iterator operator++(int) noexcept
    {
      iterator result(*this);
      ++(*this);
      return result;
    }

    iterator& operator--() noexcept
    {
      --index_;
      while (index_ > 0 && table_->slots_[index_].status != Table::Status::OCCUPIED)
      {
        --index_;
      }
      return *this;
    }

    iterator operator--(int) noexcept
    {
      iterator result(*this);
      --(*this);
      return result;
    }

    ref operator*() const noexcept
    {
      return table_->slots_[index_].pair;
    }

    point operator->() const noexcept
    {
      return std::addressof(table_->slots_[index_].pair);
    }

    bool operator==(const iterator& other) const noexcept
    {
      return table_ == other.table_ && index_ == other.index_;
    }

    bool operator!=(const iterator& other) const noexcept
    {
      return !(*this == other);
    }
  private:
    TablePtr table_;
    size_t index_;
    friend class IteratorHash< Key, Value, Hash, Equal, !IsConst >;
    friend class HashTable< Key, Value, Hash, Equal >;
  };
}
#endif
