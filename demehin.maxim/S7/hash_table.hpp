#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <functional>
#include <dynamic_array.hpp>
#include "hash_table_iterator.hpp"

namespace demehin
{
  template < typename Key, typename T, typename Hash = std::hash< Key >, typename Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class HashTIterator< Key, T, Hash, Equal, false >;
    friend class HashTIterator< Key, T, Hash, Equal, true >;
  public:
    using Iter = HashTIterator< Key, T, Hash, Equal, false >;
    using cIter = HashTIterator< Key, T, Hash, Equal, true >;

    HashTable();

    template< typename InputIt >
    HashTable(InputIt, InputIt);

    explicit HashTable(std::initializer_list< std::pair< Key, T > >);

    T& operator[](const Key&);
    const T& operator[](const Key&) const;

    std::pair< Iter, bool > insert(const std::pair< Key, T >&);
    Iter insert(cIter, const Key&);

    template< typename InputIt >
    void insert(InputIt, InputIt);

    template< typename... Args >
    std::pair< Iter, bool > emplace(Args&&...);

    template< typename... Args >
    Iter emplace_hint(cIter, Args&&...);

    size_t erase(const Key&) noexcept;
    Iter erase(cIter) noexcept;
    Iter erase(cIter, cIter) noexcept;

    void clear() noexcept;

    void swap(HashTable&) noexcept;

    T& at(const Key&);
    const T& at(const Key&) const;

    Iter find(const Key&);
    cIter find(const Key&) const;

    void rehash(size_t);
    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float);

    Iter begin() noexcept;
    Iter end() noexcept;
    cIter cbegin() const noexcept;
    cIter cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

  private:
    enum class SlotState
    {
      EMPTY,
      OCCUPIED,
      DELETED
    };

    struct Slot
    {
      std::pair< Key, T > pair;
      SlotState state = SlotState::EMPTY;
    };

    DynamicArray< Slot > slots_;
    size_t item_cnt_;
    Hash hasher_;
    Equal equal_;
    float max_load_factor_ = 0.7;

    size_t findKey(const Key&) const;
    size_t findSlot(const Key&) const;
    size_t findSlotIn(const Key&, const DynamicArray< Slot >&) const noexcept;

  };

  template< typename Key, typename T, typename Hash, typename Equal >
  HashTable< Key, T, Hash, Equal >::HashTable():
    slots_(16),
    item_cnt_(0)
  {}

  template< typename Key, typename T, typename Hash, typename Equal >
  template< typename InputIt >
  HashTable< Key, T, Hash, Equal >::HashTable(InputIt first, InputIt last):
    HashTable()
  {
    for (auto it = first; it != last; it++)
    {
      insert(*it);
    }
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  HashTable< Key, T, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, T > > ilist):
    HashTable()
  {
    for (auto it = ilist.begin(); it != ilist.end(); it++)
    {
      insert(*it);
    }
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  size_t HashTable< Key, T, Hash, Equal >::findKey(const Key& key) const
  {
    size_t hash_val = hasher_(key) % slots_.size();
    size_t ind = hash_val;
    size_t i = 1;

    while (slots_[ind].state != SlotState::EMPTY)
    {
      if (slots_[ind].state == SlotState::OCCUPIED && equal_(slots_[ind].pair.first, key))
      {
        return ind;
      }
      ind = (hash_val + i * i) % slots_.size();
      i++;

      if (i > slots_.size())
      {
        break;
      }
    }
    return slots_.size();
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  size_t HashTable< Key, T, Hash, Equal >::findSlot(const Key& key) const
  {
    size_t hash_val = hasher_(key) % slots_.size();
    size_t first_deleted = slots_.size();
    size_t ind = hash_val;
    size_t i = 1;

    while (slots_[ind].state != SlotState::EMPTY)
    {
      if (slots_[ind].state == SlotState::OCCUPIED && equal_(slots_[ind].pair.first, key))
      {
        return ind;
      }

      if (slots_[ind].state == SlotState::DELETED && first_deleted == slots_.size())
      {
        first_deleted = ind;
      }

      ind = (hash_val + i * i) % slots_.size();
      i++;

      if (i > slots_.size())
      {
        break;
      }
    }
    return (first_deleted != slots_.size()) ? first_deleted : ind;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  size_t HashTable< Key, T, Hash, Equal >::findSlotIn(const Key& key, const DynamicArray< Slot >& target) const noexcept
  {
    size_t hash_val = hasher_(key) % target.size();
    size_t ind = hash_val;
    size_t i = 1;

    while (target[ind].state == SlotState::OCCUPIED)
    {
      ind = (hash_val + i * i) % target.size();
      i++;
    }
    return ind;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  void HashTable< Key, T, Hash, Equal >::rehash(size_t new_size)
  {
    if (new_size < item_cnt_ / max_load_factor_)
    {
      new_size = static_cast< size_t >(item_cnt_ / max_load_factor_) + 1;
    }
    if (new_size <= slots_.size())
    {
      return;
    }

    DynamicArray< Slot > new_slots(new_size);
    for (auto&& slot: slots_)
    {
      if (slot.state == SlotState::OCCUPIED)
      {
        size_t ind = findSlotIn(slot.pair.first, new_slots);
        new_slots[ind] = slot;
        new_slots[ind].state = SlotState::OCCUPIED;
      }
    }
    slots_ = std::move(new_slots);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  float HashTable< Key, T, Hash, Equal >::load_factor() const noexcept
  {
    return static_cast< float >(item_cnt_) / slots_.size();
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  float HashTable< Key, T, Hash, Equal >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  void HashTable< Key, T, Hash, Equal >::max_load_factor(float ml)
  {
    max_load_factor_ = ml;
    if (load_factor() > max_load_factor_)
    {
      rehash(slots_.size() * 2);
    }
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  size_t HashTable< Key, T, Hash, Equal >::size() const noexcept
  {
    return item_cnt_;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  bool HashTable< Key, T, Hash, Equal >::empty() const noexcept
  {
    return item_cnt_ == 0;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  std::pair< typename HashTable< Key, T, Hash, Equal >::Iter, bool > HashTable< Key, T, Hash, Equal >::insert(const std::pair< Key, T >& val)
  {
    const Key& key = val.first;

    if (load_factor() >= max_load_factor_)
    {
      rehash(slots_.size() * 2);
    }

    size_t ind = findSlot(key);
    Slot& slot = slots_[ind];

    if (slot.state == SlotState::OCCUPIED)
    {
      return { Iter(this, ind), false };
    }

    slot.pair = val;
    slot.state = SlotState::OCCUPIED;
    item_cnt_++;

    return { Iter(this, ind), true };
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::Iter HashTable< Key, T, Hash, Equal >::insert(cIter hint, const Key& key)
  {
    return emplace_hint(hint, key);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  template< typename InputIt >
  void HashTable< Key, T, Hash, Equal >::insert(InputIt first, InputIt last)
  {
    HashTable< Key, T, Hash, Equal > temp(*this);
    for (auto it = first; it != last; it++)
    {
      temp.insert(*it);
    }
    swap(temp);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  template< typename... Args >
  std::pair< typename HashTable< Key, T, Hash, Equal >::Iter, bool > HashTable< Key, T, Hash, Equal >::emplace(Args&&... args)
  {
    std::pair< Key, T > tempPair(std::forward< Args >(args)...);
    const Key& key = tempPair.first;

    if (load_factor() >= max_load_factor_)
    {
      rehash(slots_.size() * 2);
    }

    size_t index = findSlot(key);
    Slot& slot = slots_[index];

    if (slot.state == SlotState::OCCUPIED)
    {
      return { Iter(this, index), false };
    }

    new (&slot.pair) std::pair< Key, T >(std::move(tempPair));
    slot.state = SlotState::OCCUPIED;
    item_cnt_++;

    return { Iter(this, index), true };
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  template< typename... Args >
  typename HashTable< Key, T, Hash, Equal >::Iter HashTable< Key, T, Hash, Equal >::emplace_hint(cIter hint, Args&&... args)
  {
    std::pair< Key, T > tempPair(std::forward< Args >(args)...);
    const Key& key = tempPair.first;

    if (hint != cend() && hint.table_ == this)
    {
      size_t hint_ind = hint.index_;
      Slot& hint_slot = slots_[hint_ind];

      if (hint_slot.state == SlotState::OCCUPIED && equal_(hint_slot.pair.first, key))
      {
        return hint;
      }
    }

    return emplace(std::move(tempPair)).first;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  T& HashTable< Key, T, Hash, Equal >::operator[](const Key& key)
  {
    auto res = insert(std::make_pair(key, T()));
    return res.first->second;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  const T& HashTable< Key, T, Hash, Equal >::operator[](const Key& key) const
  {
    return at(key);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  T& HashTable< Key, T, Hash, Equal >::at(const Key& key)
  {
    size_t ind = findKey(key);
    if (ind == slots_.size())
    {
      throw std::logic_error("key not found");
    }

    return slots_[ind].pair.second;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  const T& HashTable< Key, T, Hash, Equal >::at(const Key& key) const
  {
    size_t ind = find_key(key);
    if (ind == slots_.size())
    {
      throw std::logic_error("key not found");
    }

    return slots_[ind].pair.second;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::Iter HashTable< Key, T, Hash, Equal >::begin() noexcept
  {
    return Iter(this, 0);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::Iter HashTable< Key, T, Hash, Equal >::end() noexcept
  {
    return Iter(this, slots_.size());
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::cIter HashTable< Key, T, Hash, Equal >::cbegin() const noexcept
  {
    return cIter(this, 0);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::cIter HashTable< Key, T, Hash, Equal >::cend() const noexcept
  {
    return cIter(this, slots_.size());
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::Iter HashTable< Key, T, Hash, Equal >::find(const Key& key)
  {
    size_t ind = findKey(key);
    return (ind == slots_.size()) ? end() : Iter(this, ind);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::cIter HashTable< Key, T, Hash, Equal >::find(const Key& key) const
  {
    size_t ind = findKey(key);
    return (ind == slots_.size()) ? cend() : cIter(this, ind);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  size_t HashTable< Key, T, Hash, Equal >::erase(const Key& key) noexcept
  {
    cIter it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::Iter HashTable< Key, T, Hash, Equal >::erase(cIter pos) noexcept
  {
    slots_[pos.index_].state = SlotState::DELETED;
    item_cnt_--;

    size_t next_ind = pos.index_ + 1;
    while (next_ind < slots_.size() && slots_[next_ind].state != SlotState::OCCUPIED)
    {
      next_ind++;
    }

    return Iter(this, next_ind);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  typename HashTable< Key, T, Hash, Equal >::Iter HashTable< Key, T, Hash, Equal >::erase(cIter first, cIter last) noexcept
  {
    auto it = first;
    while (it != last)
    {
      it = erase(it);
    }
    return Iter(this, first.index_);
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  void HashTable< Key, T, Hash, Equal >::clear() noexcept
  {
    erase(cbegin(), cend());
  }

  template< typename Key, typename T, typename Hash, typename Equal >
  void HashTable< Key, T, Hash, Equal >::swap(HashTable& other) noexcept
  {
    std::swap(slots_, other.slots_);
    std::swap(item_cnt_, other.item_cnt_);
    std::swap(hasher_, other.hasher_);
    std::swap(equal_, other.equal_);
  }
}

#endif
