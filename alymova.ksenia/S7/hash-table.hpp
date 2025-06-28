#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <functional>
#include <cmath>
#include <cassert>
#include "hash-node.hpp"
#include "hash-iterators.hpp"

namespace alymova
{
  template< class Key, class Value, class Hash = std::hash< Key >, class KeyEqual = std::equal_to< Key > >
  class HashTable
  {
  public:
    using Hashtable = HashTable< Key, Value, Hash, KeyEqual >;
    using Iterator = HashIterator< Key, Value, Hash, KeyEqual >;
    using ConstIterator = HashConstIterator< Key, Value, Hash, KeyEqual >;
    using Node = detail::HashNode< Key, Value >;
    using ValueType = std::pair< Key, Value >;

    HashTable();
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    template< class InputIterator >
    HashTable(InputIterator first, InputIterator last);
    HashTable(std::initializer_list< ValueType > il);
    ~HashTable() noexcept;

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator cbegin() const noexcept;
    Iterator end() noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    Iterator insert(const ValueType& value);
    Iterator insert(ValueType&& value);
    template< class InputIterator >
    void insert(InputIterator first, InputIterator last);
    Iterator insert(ConstIterator hint, const ValueType& value);

    template< class... Args >
    Iterator emplace(Args&&... args);
    template< class... Args >
    Iterator emplace_hint(ConstIterator hint, Args&&... args);

    size_t erase(const Key& key);
    Iterator erase(Iterator pos);
    Iterator erase(ConstIterator pos);
    Iterator erase(ConstIterator first, ConstIterator last);

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;

    void clear() noexcept;
    void swap(HashTable& other);

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float mlf);
    void rehash();
  private:
    enum NodeState: int {Empty, Fill};
    using T = std::pair< NodeState, Node >;

    float max_load_factor_ = 0.7;
    size_t size_;
    size_t capacity_;
    T* array_;
    Hash hasher_;
    KeyEqual equal_;

    size_t get_home_index(const Key& key) const noexcept;
    std::pair< size_t, T > rob_rich(size_t home_index, const Node& value);
    Iterator insert_node(size_t index, const Node& node);
    size_t get_next_prime_capacity() const noexcept;
    void clear_default() noexcept;

    friend class HashConstIterator< Key, Value, Hash, KeyEqual >;
    friend class HashIterator< Key, Value, Hash, KeyEqual >;
  };

  template< class Key, class Value, class Hash, class KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable():
    size_(0),
    capacity_(11),
    array_(new T[capacity_]),
    hasher_(),
    equal_()
  {
    clear_default();
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(const HashTable& other):
    size_(other.size_),
    capacity_(other.capacity_),
    array_(new T[capacity_]),
    hasher_(other.hasher_),
    equal_()
  {
    clear_default();
    for (size_t i = 0; i != other.capacity_; i++)
    {
      if (other.array_[i].first == NodeState::Fill)
      {
        try
        {
          array_[i] = other.array_[i];
        }
        catch (...)
        {
          delete[] array_;
          throw;
        }
      }
    }
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(HashTable&& other) noexcept:
    size_(std::exchange(other.size_, 0)),
    capacity_(std::exchange(other.capacity_, 0)),
    array_(std::exchange(other.array_, nullptr)),
    hasher_(std::exchange(other.hasher_, Hash())),
    equal_(std::exchange(other.equal_, KeyEqual()))
  {}

  template< class Key, class Value, class Hash, class KeyEqual >
  template< class InputIterator >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(InputIterator first, InputIterator last):
    HashTable()
  {
    insert(first, last);
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(std::initializer_list< ValueType > il):
    HashTable(il.begin(), il.end())
  {}


  template< class Key, class Value, class Hash, class KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::~HashTable() noexcept
  {
    delete[] array_;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >&
    HashTable< Key, Value, Hash, KeyEqual >::operator=(const HashTable& other)
  {
    HashTable copy(other);
    swap(copy);
    return *this;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >&
    HashTable< Key, Value, Hash, KeyEqual >::operator=(HashTable&& other) noexcept
  {
    HashTable movy(std::move(other));
    swap(movy);
    return *this;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::begin() noexcept
  {
    ConstIterator it = cbegin();
    return Iterator{it.node_, it.end_};
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashConstIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::begin() const noexcept
  {
    return cbegin();
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashConstIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::cbegin() const noexcept
  {
    ConstIterator it{array_, array_ + capacity_};
    if (array_->first == NodeState::Empty)
    {
      it++;
    }
    return it;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::end() noexcept
  {
    return Iterator{array_ + capacity_, array_ + capacity_};
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashConstIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::end() const noexcept
  {
    return cend();
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashConstIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::cend() const noexcept
  {
    return ConstIterator{array_ + capacity_, array_ + capacity_};
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  bool HashTable< Key, Value, Hash, KeyEqual >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  size_t HashTable< Key, Value, Hash, KeyEqual >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::insert(const ValueType& value)
  {
    return emplace(value);
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::insert(ValueType&& value)
  {
    return emplace(std::forward< ValueType >(value));
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  template< class InputIterator >
  void HashTable< Key, Value, Hash, KeyEqual >::insert(InputIterator first, InputIterator last)
  {
    for (; first != last; first++)
    {
      emplace(*first);
    }
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual >
    HashTable< Key, Value, Hash, KeyEqual >::insert(ConstIterator hint, const ValueType& value)
  {
    return emplace_hint(hint, std::forward< ValueType >(value));
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  template< class... Args >
  HashIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::emplace(Args&&... args)
  {
    ValueType value(std::forward< Args >(args)...);
    size_t home_index = get_home_index(value.first);
    Node node{value, 0};
    Iterator res = insert_node(home_index, node);
    size_++;
    if (size_ > max_load_factor_ * capacity_)
    {
      rehash();
      return find(value.first);
    }
    return res;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  template< class... Args >
  HashIterator< Key, Value, Hash, KeyEqual >
    HashTable< Key, Value, Hash, KeyEqual >::emplace_hint(ConstIterator hint, Args&&... args)
  {
    ValueType value(std::forward< Args >(args)...);
    if (hint != end())
    {
      size_t hint_index = hint.node_ - array_;
      if (array_[hint_index].first == NodeState::Fill)
      {
        if (equal_(array_[hint_index].second.get_key(), value.first))
        {
          return {hint.node_, hint.end_};
        }
      }
    }
    return emplace(std::move(value));
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  size_t HashTable< Key, Value, Hash, KeyEqual >::erase(const Key& key)
  {
    size_t size_old = size_;
    while (find(key) != end())
    {
      erase(find(key));
    }
    return size_old - size_;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual > HashTable< Key, Value, Hash, KeyEqual >::erase(Iterator pos)
  {
    assert(size_ != 0 && "You try to delete from empty container");
    assert(pos != end() && "You try to delete beyond table's bound");

    pos.node_->first = NodeState::Empty;
    size_t i = (pos.node_ - array_ + 1) % capacity_;
    while (array_[i].first != NodeState::Empty)
    {
      if (array_[i].second.psl == 0)
      {
        break;
      }
      size_t prev = i - 1;
      if (i == 0)
      {
        prev = capacity_ - 1;
      }
      array_[i].second.psl--;
      std::swap(array_[i], array_[prev]);
      i = (i + 1) % capacity_;
    }
    if (pos.node_->first == NodeState::Empty)
    {
      pos++;
    }
    size_--;
    return pos;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual >
    HashTable< Key, Value, Hash, KeyEqual >::erase(ConstIterator pos)
  {
    Iterator it(pos.node_, pos.end_);
    return erase(it);
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual >
    HashTable< Key, Value, Hash, KeyEqual >::erase(ConstIterator first, ConstIterator last)
  {
    while (first != last)
    {
      first = erase(first);
    }
    return Iterator{first.node_, first.end_};
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  Value& HashTable< Key, Value, Hash, KeyEqual >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const HashTable& >(*this).at(key));
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  const Value& HashTable< Key, Value, Hash, KeyEqual >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Container does not have an element with the such key");
    }
    return it->second;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual >
    HashTable< Key, Value, Hash, KeyEqual >::find(const Key& key)
  {
    ConstIterator it = static_cast< const HashTable& >(*this).find(key);
    return Iterator{it.node_, it.end_};
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashConstIterator< Key, Value, Hash, KeyEqual >
    HashTable< Key, Value, Hash, KeyEqual >::find(const Key& key) const
  {
    size_t home_index = get_home_index(key);
    if (array_[home_index].first == NodeState::Empty)
    {
      return end();
    }
    if (equal_(array_[home_index].second.get_key(), key))
    {
      return ConstIterator{array_ + home_index, array_ + capacity_};
    }
    size_t psl = 1;
    for (size_t i = (home_index + 1) % capacity_; i != home_index; i = (i + 1) % capacity_)
    {
      if (array_[i].first == NodeState::Empty)
      {
        return end();
      }
      if (array_[i].second.psl < psl)
      {
        return end();
      }
      if (equal_(array_[i].second.get_key(), key))
      {
        return ConstIterator{array_ + i, array_ + capacity_};
      }
      psl++;
    }
    return end();
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::clear() noexcept
  {
    clear_default();
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::swap(HashTable& other)
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(array_, other.array_);
    std::swap(hasher_, other.hasher_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  float HashTable< Key, Value, Hash, KeyEqual >::load_factor() const noexcept
  {
    return (size_ * 1.0) / (capacity_ * 1.0);
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  float HashTable< Key, Value, Hash, KeyEqual >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::max_load_factor(float mlf)
  {
    max_load_factor_ = mlf;
    if (size_ > capacity_ * max_load_factor_)
    {
      rehash();
    }
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::rehash()
  {
    size_t capacity_old = capacity_;
    capacity_ = get_next_prime_capacity();
    T* array_new = nullptr;
    try
    {
      array_new = new T[capacity_];
      std::swap(array_new, array_);
      clear();
      for (size_t i = 0; i < capacity_old; i++)
      {
        if (array_new[i].first == NodeState::Fill)
        {
          insert(array_new[i].second.data);
        }
      }
      delete[] array_new;
    }
    catch (...)
    {
      delete[] array_new;
      capacity_ = capacity_old;
      throw;
    }
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  size_t HashTable< Key, Value, Hash, KeyEqual >::get_home_index(const Key& key) const noexcept
  {
    return hasher_(key) % capacity_;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  std::pair< size_t, typename HashTable< Key, Value, Hash, KeyEqual >::T >
    HashTable< Key, Value, Hash, KeyEqual >::rob_rich(size_t home_index, const Node& node)
  {
    std::pair< NodeState, Node > node_t{NodeState::Fill, node};
    node_t.second.psl++;
    for (size_t i = (home_index + 1) % capacity_; i != home_index; i = (i + 1) % capacity_)
    {
      if (array_[i].first == NodeState::Empty)
      {
        node_t.swap(array_[i]);
        return {i, node_t};
      }
      if (node_t.second.psl > array_[i].second.psl)
      {
        node_t.swap(array_[i]);
        return {i, node_t};
      }
      node_t.second.psl++;
    }
    return {home_index, node_t};
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  HashIterator< Key, Value, Hash, KeyEqual >
    HashTable< Key, Value, Hash, KeyEqual >::insert_node(size_t index, const Node& node)
  {
    size_t home_index;
    if (index < node.psl)
    {
      home_index = capacity_ - node.psl;
    }
    else
    {
      home_index = index - node.psl;
    }
    if (array_[home_index].first == NodeState::Empty)
    {
      array_[home_index].first = NodeState::Fill;
      array_[home_index].second = node;
      return Iterator{array_ + home_index, array_ + capacity_};
    }
    std::pair< size_t, T > rob_res = rob_rich(home_index, node);
    if (rob_res.first == home_index)
    {
      rehash();
    }
    if (rob_res.second.first == NodeState::Fill)
    {
      return insert_node(rob_res.first, rob_res.second.second);
    }
    return Iterator{array_ + rob_res.first, array_ + capacity_};
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  size_t HashTable< Key, Value, Hash, KeyEqual >::get_next_prime_capacity() const noexcept
  {
    size_t capacity_new = capacity_ * 2;
    for (size_t i = capacity_new + 1; i < std::pow(10, 7); i += 2)
    {
      bool prime = true;
      for (size_t del = 3; del < std::round(std::sqrt(capacity_new)) + 1; del++)
      {
        if (i % del == 0)
        {
          prime = false;
          break;
        }
      }
      if (prime)
      {
        return i;
      }
    }
    return capacity_new;
  }

  template< class Key, class Value, class Hash, class KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::clear_default() noexcept
  {
    for (size_t i = 0; i < capacity_; i++)
    {
      array_[i].first = NodeState::Empty;
    }
  }
}

#endif
