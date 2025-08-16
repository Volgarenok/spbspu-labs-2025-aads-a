#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <boost/container_hash/hash.hpp>
#include "slot.hpp"
#include "iterator.hpp"

namespace aleksandrov
{
  constexpr size_t minCapacity = 61;
  constexpr float maxLoadFactorValue = 0.75f;

  template< class K, class V, class H = boost::hash< K >, class E = std::equal_to< K > >
  class HashTable
  {
  public:
    using Iter = Iterator< K, V, H, E, false >;
    using ConstIter = Iterator< K, V, H, E, true >;
    using ValueType = std::pair< K, V >;

    HashTable();
    template< class InputIt >
    HashTable(InputIt, InputIt);
    HashTable(std::initializer_list< ValueType >);
    HashTable(const HashTable&);
    HashTable(HashTable&&);
    ~HashTable() noexcept;

    HashTable& operator=(const HashTable&);
    HashTable& operator=(HashTable&&);
    HashTable& operator=(std::initializer_list< ValueType >);

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    void clear() noexcept;
    std::pair< Iter, bool > insert(const ValueType&);
    std::pair< Iter, bool > insert(ValueType&&);
    Iter insert(ConstIter, const ValueType&);
    Iter insert(ConstIter, ValueType&&);
    template< class InputIt >
    void insert(InputIt, InputIt);
    void insert(std::initializer_list< ValueType >);
    template< class... Args >
    std::pair< Iter, bool > emplace(Args&&...);
    template< class... Args >
    Iter emplaceHint(ConstIter, Args&&...);
    void swap(HashTable&) noexcept;

    V& at(const K&);
    const V& at(const K&) const;
    V& operator[](const K&);
    V& operator[](K&&);
    size_t count(const K&) const;
    Iter find(const K&);
    ConstIter find(const K&) const;

    Iter begin();
    ConstIter begin() const;
    ConstIter cbegin() const;
    Iter end();
    ConstIter end() const;
    ConstIter cend() const;

    float loadFactor() const noexcept;
    float maxLoadFactor() const noexcept;
    void maxLoadFactor(float) noexcept;
    void rehash(size_t);

    H hashFunction() const;
    E keyEq() const;

  private:
    template< class, class, class, class, bool >
    friend class Iterator;
    using Slot = detail::Slot< K, V >;

    Slot* data_;
    size_t size_;
    size_t capacity_;
    float maxLoadFactor_;
    H hasher_;
    E keyEqual_;

    Slot* copyData(const HashTable&);
    Iter emplaceInternal(Slot);
    void rehashToCapacity(size_t);
    void rehash();
  };

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable():
    data_(nullptr),
    size_(0),
    capacity_(0),
    maxLoadFactor_(maxLoadFactorValue),
    hasher_{},
    keyEqual_{}
  {}

  template< class K, class V, class H, class E >
  template< class InputIt >
  HashTable< K, V, H, E >::HashTable(InputIt first, InputIt last):
    HashTable()
  {
    for (auto it = first; it != last; ++it)
    {
      emplace(*it);
    }
  }

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable(std::initializer_list< ValueType > ilist):
    HashTable(ilist.begin(), ilist.end())
  {}

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable(const HashTable& rhs):
    data_(copyData(rhs)),
    size_(rhs.size_),
    capacity_(rhs.capacity_),
    maxLoadFactor_(rhs.maxLoadFactor_),
    hasher_(rhs.hasher_),
    keyEqual_(rhs.keyEqual_)
  {}

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable(HashTable&& rhs):
    data_(std::exchange(rhs.data_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0)),
    maxLoadFactor_(std::exchange(rhs.maxLoadFactor_, 0)),
    hasher_(std::exchange(rhs.hasher_, H{})),
    keyEqual_(std::exchange(rhs.keyEqual_, E{}))
  {}

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::operator=(const HashTable& rhs) -> HashTable&
  {
    HashTable temp(rhs);
    swap(temp);
    return *this;
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::operator=(HashTable&& rhs) -> HashTable&
  {
    HashTable temp(std::move(rhs));
    swap(temp);
    return *this;
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::operator=(std::initializer_list< ValueType > ilist) -> HashTable&
  {
    HashTable temp(ilist);
    swap(temp);
    return *this;
  }

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::~HashTable() noexcept
  {
    delete[] data_;
  }

  template< class K, class V, class H, class E >
  bool HashTable< K, V, H, E >::empty() const noexcept
  {
    return !size_;
  }

  template< class K, class V, class H, class E >
  size_t HashTable< K, V, H, E >::size() const noexcept
  {
    return size_;
  }

  template< class K, class V, class H, class E >
  size_t HashTable< K, V, H, E >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::clear() noexcept
  {
    if (!empty())
    {
      for (size_t i = 0; i < capacity_; ++i)
      {
        data_[i].occupied = false;
      }
      size_ = 0;
    }
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::insert(const ValueType& value) -> std::pair< Iter, bool >
  {
    return emplace(value);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::insert(ValueType&& value) -> std::pair< Iter, bool >
  {
    return emplace(std::move(value));
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::insert(ConstIter hint, const ValueType& value) -> Iter
  {
    return emplaceHint(hint, value);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::insert(ConstIter hint, ValueType&& value) -> Iter
  {
    return emplaceHint(hint, std::move(value));
  }

  template< class K, class V, class H, class E >
  template< class InputIt >
  void HashTable< K, V, H, E >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::insert(std::initializer_list< ValueType > ilist)
  {
    insert(ilist.begin(), ilist.end());
  }

  template< class K, class V, class H, class E >
  template< class... Args >
  auto HashTable< K, V, H, E >::emplace(Args&&... args) -> std::pair< Iter, bool >
  {
    if (empty())
    {
      rehash();
    }
    else if (loadFactor() > maxLoadFactor())
    {
      rehash();
    }
    Slot newSlot(std::forward< Args >(args)...);
    Iter it = find(newSlot.data.first);
    if (it != end())
    {
      return { it, false };
    }
    Iter emplaced = emplaceInternal(std::move(newSlot));
    return { emplaced, true };
  }

  template< class K, class V, class H, class E >
  template< class... Args >
  auto HashTable< K, V, H, E >::emplaceHint(ConstIter hint, Args&&... args) -> Iter
  {
    if (empty())
    {
      rehash();
    }
    else if (loadFactor() > maxLoadFactor())
    {
      rehash();
    }
    Slot newSlot(std::forward< Args >(args)...);
    Iter it = find(newSlot.data.first);
    if (it != end())
    {
      return it;
    }
    return emplaceInternal(std::move(newSlot));
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::swap(HashTable& rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(maxLoadFactor_, rhs.maxLoadFactor_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(keyEqual_, rhs.keyEqual_);
  }

  template< class K, class V, class H, class E >
  V& HashTable< K, V, H, E >::at(const K& key)
  {
    return const_cast< V& >(static_cast< const HashTable& >(*this).at(key));
  }

  template< class K, class V, class H, class E >
  const V& HashTable< K, V, H, E >::at(const K& key) const
  {
    ConstIter it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("ERROR: No such element exists!");
    }
    return it->second;
  }

  template< class K, class V, class H, class E >
  V& HashTable< K, V, H, E >::operator[](const K& key)
  {
    return const_cast< V& >(static_cast< const HashTable& >(*this).operator[](key));
  }

  template< class K, class V, class H, class E >
  V& HashTable< K, V, H, E >::operator[](K&& key)
  {
    Iter it = find(key);
    if (it != end())
    {
      return it->second;
    }
    return insert({ std::move(key), V{} }).first->second;
  }

  template< class K, class V, class H, class E >
  size_t HashTable< K, V, H, E >::count(const K& key) const
  {
    return find(key) != cend();
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::find(const K& key) -> Iter
  {
    ConstIter it = static_cast< const HashTable& >(*this).find(key);
    if (it == cend())
    {
      return end();
    }
    return Iter(this, it.index_);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::find(const K& key) const -> ConstIter
  {
    if (empty())
    {
      return cend();
    }
    size_t i = hasher_(key) % capacity_;
    size_t psl = 0;
    size_t curr = i;
    while (data_[curr].occupied && data_[curr].psl >= psl)
    {
      if (keyEqual_(data_[curr].data.first, key))
      {
        return ConstIter(this, curr);
      }
      ++psl;
      curr = (curr + 1) % capacity_;
      if (curr == i)
      {
        return cend();
      }
    }
    return cend();
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::begin() -> Iter
  {
    return Iter(this);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::begin() const -> ConstIter
  {
    return cbegin();
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::cbegin() const -> ConstIter
  {
    return ConstIter(this);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::end() -> Iter
  {
    return Iter(this, capacity_);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::end() const -> ConstIter
  {
    return cend();
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::cend() const -> ConstIter
  {
    return ConstIter(this, capacity_);
  }

  template< class K, class V, class H, class E >
  float HashTable< K, V, H, E >::loadFactor() const noexcept
  {
    if (!capacity_)
    {
      return 0.0f;
    }
    return static_cast< float >(size_) / capacity_;
  }

  template< class K, class V, class H, class E >
  float HashTable< K, V, H, E >::maxLoadFactor() const noexcept
  {
    return maxLoadFactor_;
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::maxLoadFactor(float ml) noexcept
  {
    maxLoadFactor_ = ml;
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::rehash(size_t count)
  {
    size_t minRequiredCapacity = static_cast< size_t >(size() / maxLoadFactor());
    size_t newCapacity = std::max(count, minRequiredCapacity);
    rehashToCapacity(newCapacity);
  }

  template< class K, class V, class H, class E >
  H HashTable< K, V, H, E >::hashFunction() const
  {
    return hasher_;
  }

  template< class K, class V, class H, class E >
  E HashTable< K, V, H, E >::keyEq() const
  {
    return keyEqual_;
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::copyData(const HashTable& hashtable) -> Slot*
  {
    Slot* copy = new Slot[hashtable.capacity_];
    try
    {
      for (size_t i = 0; i < hashtable.capacity_; ++i)
      {
        if (hashtable.data_[i].occupied)
        {
          copy[i] = hashtable.data_[i];
        }
      }
    }
    catch (...)
    {
      delete[] copy;
      throw;
    }
    return copy;
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::emplaceInternal(Slot newSlot) -> Iter
  {
    size_t i = hasher_(newSlot.data.first) % capacity_;
    while (data_[i].occupied)
    {
      if (newSlot.psl > data_[i].psl)
      {
        std::swap(data_[i], newSlot);
      }
      ++newSlot.psl;
      i = (i + 1) % capacity_;
    }
    data_[i] = newSlot;
    ++size_;
    return Iter(this, i);
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::rehashToCapacity(size_t newCapacity)
  {
    if (newCapacity <= capacity_)
    {
      return;
    }
    HashTable temp;
    temp.capacity_ = newCapacity;
    temp.data_ = new Slot[newCapacity];
    temp.size_ = 0;
    temp.maxLoadFactor_ = maxLoadFactor_;
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (data_[i].occupied)
      {
        temp.emplaceInternal(std::move(data_[i].data));
      }
    }
    swap(temp);
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::rehash()
  {
    size_t newCapacity = capacity_ ? capacity_ * 2 : minCapacity;
    rehashToCapacity(newCapacity);
  }
}

#endif

