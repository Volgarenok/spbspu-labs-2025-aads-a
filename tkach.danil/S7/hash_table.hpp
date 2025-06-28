#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <functional>
#include "hash_dyn_array.hpp"
#include "hash_iter.hpp"
#include "hash_citer.hpp"

namespace tkach
{
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
  public:
    using Iter = HashIterator< Key, Value, Hash, Equal >;
    using CIter = CHashIterator< Key, Value, Hash, Equal >;
    using pair_t = std::pair< HashIterator< Key, Value, Hash, Equal >, bool >;
    HashTable();
    template< class InputIt >
    HashTable(InputIt begin, InputIt end);
    HashTable(std::initializer_list< std::pair< Key, Value > > init_list);
    HashTable(size_t size);
    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    void clear();
    void swap(HashTable< Key, Value, Hash, Equal >& rhs) noexcept;
    CIter cend() const;
    CIter cbegin() const;
    Iter end();
    Iter begin();
    size_t size() const noexcept;
    bool empty() const noexcept;
    pair_t insert(const std::pair< Key, Value >& pair);
    pair_t insert(std::pair< Key, Value >&& pair);
    Iter insert(CIter, const std::pair< Key, Value >& pair);
    Iter insert(CIter, std::pair< Key, Value >&& pair);
    template< class InputIt >
    void insert(InputIt first, InputIt last);
    Iter find(const Key& key);
    CIter find(const Key& key) const;
    size_t erase(const Key& key);
    Iter erase(Iter it);
    Iter erase(CIter it);
    Iter erase(CIter first, CIter last);
    void rehash(size_t new_size);
    float maxLoadFactor() const;
    void maxLoadFactor(float ml);
    float load_factor() const;
    template< class... Args >
    pair_t emplace(Args&&... args);
    template< class... Args >
    Iter emplace_hint(CIter hint, Args&&... args);
  private:
    enum class EntryState
    {
      Empty,
      Occupied,
      Deleted
    };
    struct Slot
    {
      std::pair< Key, Value > pair;
      EntryState state = EntryState::Empty;
    };
    HashDynArray< Slot > table_;
    size_t count_;
    Hash hasher_;
    Equal key_equal_;
    float max_load_factor_ ;
    size_t find_pos(const Key& key, const HashDynArray< Slot >& table) const;
    friend class HashIterator< Key, Value, Hash, Equal >;
    friend class CHashIterator< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    table_(11),
    count_(0),
    hasher_(Hash()),
    key_equal_(Equal()),
    max_load_factor_(0.7f)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t size):
    table_(size),
    count_(0),
    hasher_(Hash()),
    key_equal_(Equal()),
    max_load_factor_(0.7f)
  {}

  template< class Key, class Value, class Hash, class Equal >
  template< class InputIt >
  HashTable< Key, Value, Hash, Equal >::HashTable(InputIt begin, InputIt end):
    HashTable()
  {
    insert(begin, end);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear()
  {
    erase(cbegin(), cend());
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > init_list):
    HashTable(init_list.begin(), init_list.end())
  {}

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return count_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return count_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  CHashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return CIter{this, table_.size()};
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable< Key, Value, Hash, Equal >& rhs) noexcept
  {
    std::swap(table_, rhs.table_);
    std::swap(count_, rhs.count_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(key_equal_, rhs.key_equal_);
    std::swap(max_load_factor_, rhs.max_load_factor_);
  }

  template< class Key, class Value, class Hash, class Equal >
  CHashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    return CIter{this, 0};
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return Iter{this, table_.size()};
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    return Iter{this, 0};
  }

  template< class Key, class Value, class Hash, class Equal >
  auto HashTable< Key, Value, Hash, Equal >::insert(const std::pair< Key, Value >& pair) -> pair_t
  {
    return emplace(pair);
  }

  template< class Key, class Value, class Hash, class Equal >
  auto HashTable< Key, Value, Hash, Equal >::insert(std::pair< Key, Value >&& pair) -> pair_t
  {
    return emplace(std::move(pair));
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::insert(CIter cit, const std::pair< Key, Value >& pair)
  {
    return emplace_hint(cit, pair);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::insert(CIter cit, std::pair< Key, Value >&& pair)
  {
    return emplace_hint(cit, std::move(pair));
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class InputIt >
  void HashTable< Key, Value, Hash, Equal >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class... Args >
  auto HashTable< Key, Value, Hash, Equal >::emplace(Args&&... args) -> pair_t
  {
    if (((count_ + 1) * 1.0) / table_.size() >= max_load_factor_)
    {
      rehash(table_.size() * 2 + 1);
    }
    std::pair< Key, Value > pair(std::forward< Args >(args)...);
    size_t pos = find_pos(pair.first, this->table_);
    if (table_[pos].state == EntryState::Occupied)
    {
      return {HashIterator< Key, Value, Hash, Equal >{this, pos}, false};
    }
    table_[pos].pair = std::move(pair);
    table_[pos].state = EntryState::Occupied;
    count_++;
    return {HashIterator< Key, Value, Hash, Equal >{this, pos}, true};
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class... Args >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::emplace_hint(CIter hint, Args&&... args)
  {
    std::pair< Key, Value > temp_pair(std::forward< Args >(args)...);
    if (hint != cend() && table_[hint.pos_].state == EntryState::Occupied)
    {
      if (key_equal_(table_[hint.pos_].pair.first, temp_pair.first))
      {
        return Iter{this, hint.pos_};
      }
    }
    return emplace(std::move(temp_pair)).first;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::find_pos(const Key& key, const HashDynArray< Slot >& table) const
  {
    size_t initial_index = hasher_(key) % table.size();
    size_t first_deleted = table.size();
    for (size_t i = 0; i < table.size(); ++i)
    {
      size_t cur_index = (initial_index + i * i) % table.size();
      if (table[cur_index].state == EntryState::Occupied)
      {
        if (key_equal_(table[cur_index].pair.first, key))
        {
          return cur_index;
        }
      }
      else if (table[cur_index].state == EntryState::Deleted)
      {
        if (first_deleted == table.size())
        {
          first_deleted = cur_index;
        }
      }
      else
      {
        return (first_deleted != table.size()) ? first_deleted : cur_index;
      }
    }
    return first_deleted;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t new_size)
  {
    if (new_size < count_)
    {
      new_size = count_ * 2 + 1;
    }
    if (new_size == 0)
    {
      new_size = 11;
    }
    HashDynArray< Slot > new_table(new_size);
    for (size_t i = 0; i < table_.size(); ++i)
    {
      if (table_[i].state == EntryState::Occupied)
      {
        size_t new_index = find_pos(table_[i].pair.first, new_table);
        new_table[new_index].pair = table_[i].pair;
        new_table[new_index].state = EntryState::Occupied;
      }
    }
    table_.swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& key)
  {
    Iter it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::erase(Iter it)
  {
    return erase(CIter(it));
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::erase(CIter it)
  {
    if (table_[it.pos_].state == EntryState::Deleted || table_[it.pos_].state == EntryState::Empty)
    {
      return Iter(++it);
    }
    table_[it.pos_].state = EntryState::Deleted;
    --count_;
    ++it;
    return Iter{this, it.pos_};

  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::erase(CIter first, CIter last)
  {
    Iter res;
    while (first != last)
    {
      res = erase(first++);
    }
    return res;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::find(const Key& key)
  {
    return Iter(static_cast< const HashTable< Key, Value, Hash, Equal >* >(this)->find(key));
  }

  template< class Key, class Value, class Hash, class Equal >
  CHashIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::find(const Key& key) const
  {
    size_t initial_index = hasher_(key) % table_.size();
    for (size_t i = 0; i < table_.size(); ++i)
    {
      size_t cur_index = (initial_index + i * i) % table_.size();
      if (table_[cur_index].state == EntryState::Occupied)
      {
        if (key_equal_(table_[cur_index].pair.first, key))
        {
          return CIter(this, cur_index);
        }
      }
      else if (table_[cur_index].state == EntryState::Empty)
      {
        return cend();
      }
    }
    return cend();
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    std::pair< Iter, bool > it2 = insert(std::make_pair(key, Value()));
    return it2.first->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key) const
  {
    CIter cit = find(key);
    return cit->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    Iter it = find(key);
    if (it != end())
    {
      return it->second;
    }
    throw std::out_of_range("<INVALID COMMAND>");
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    CIter it = find(key);
    if (it != cend())
    {
      return it->second;
    }
    throw std::out_of_range("<INVALID COMMAND>");
  }

  template< class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::maxLoadFactor() const
  {
    return max_load_factor_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::maxLoadFactor(float ml)
  {
    if (ml <= 0.1f || ml >= 0.95f)
    {
      throw std::invalid_argument("Invalid load factor");
    }
    max_load_factor_ = ml;
    if (load_factor() >= max_load_factor_)
    {
      rehash(2 * table_.size() + 1);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::load_factor() const
  {
    return table_.empty() ? 0.0f : static_cast< float >(count_) / table_.size();
  }
}
#endif
