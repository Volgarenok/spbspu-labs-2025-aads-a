#ifndef ITERATORHASH_HPP
#define ITERATORHASH_HPP
#include <cstddef>
#include <memory>
#include <utility>
namespace kiselev
{
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash1, class Hash2, class Equal, bool IsConst >
  class IteratorHash
  {
  public:
    using value = std::pair< Key, Value >;
    using reference = std::conditional_t< IsConst, const value&, value& >;
    using pointer = std::conditional_t< IsConst, const value*, value* >;
    using iterator = IteratorHash< Key, Value, Hash1, Hash2, Equal, IsConst >;
    using Table = HashTable< Key, Value, Hash1, Hash2, Equal >;
    using constTable = const HashTable< Key, Value, Hash1, Hash2, Equal >;
    using TablePtr = std::conditional_t< IsConst, constTable*, Table* >;

    IteratorHash():
      table_(nullptr),
      index_(0)
    {}

    template < bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    IteratorHash(const IteratorHash< Key, Value, Hash1, Hash2, Equal, OtherIsConst >& other) noexcept:
      table_(other.table_),
      index_(other.index_)
    {}

    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    iterator& operator=(const IteratorHash< Key, Value, Hash1, Hash2, Equal, OtherIsConst >& other) noexcept
    {
      table_ = other.table_;
      index_ = other.index_;
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

    reference operator*() const noexcept
    {
      return table_->slots_[index_].pair;
    }
    pointer operator->() const noexcept
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
    IteratorHash(TablePtr tab, size_t index) noexcept:
      table_(tab),
      index_(index)
    {}

    friend class IteratorHash< Key, Value, Hash1, Hash2, Equal, !IsConst >;
    friend class HashTable< Key, Value, Hash1, Hash2, Equal >;
  };
}
#endif
