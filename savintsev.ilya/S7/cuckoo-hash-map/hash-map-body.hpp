#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP
#include <initializer_list>
#include <iterator>
#include <iostream>
#include <vector>
#include <boost/hash2/xxhash.hpp>
#include <dynamic-array.hpp>
#include "hash-wrapper.hpp"

namespace savintsev
{
  template
  <
    typename Key,
    typename T,
    typename HS1 = std::hash< Key >,
    typename HS2 = Hash< Key, boost::hash2::xxhash_64 >,
    typename EQ = std::equal_to<>
  >
  class HashMap
  {
  public:
    class FwdConstIter;

    class FwdIter
    {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair< Key, T >;
      using difference_type = std::ptrdiff_t;
      using pointer = value_type *;
      using reference = value_type &;

      friend class FwdConstIter;
      friend class HashMap;

      FwdIter() = default;
      FwdIter(const FwdConstIter & other):
        parent_(const_cast< HashMap * >(other.parent_)),
        pos_(other.pos_),
        first_(other.first_)
      {
        skip_empty();
      }
      FwdIter(HashMap * parent, size_t pos, bool in_table1):
        parent_(parent),
        pos_(pos),
        first_(in_table1)
      {
        skip_empty();
      }

      reference operator*();
      pointer operator->();
      reference operator*() const;
      pointer operator->() const;

      FwdIter & operator++();
      FwdIter operator++(int);

      bool operator!=(const FwdIter & rhs) const;
      bool operator==(const FwdIter & rhs) const;

      friend std::ostream & operator<<(std::ostream & os, const FwdConstIter & iter)
      {
        return os << iter->first;
      }
    private:
      HashMap * parent_ = nullptr;
      size_t pos_ = 0;
      bool first_ = true;
      void skip_empty();
    };

    class FwdConstIter
    {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair< Key, T >;
      using difference_type = std::ptrdiff_t;
      using pointer = const value_type *;
      using reference = const value_type &;

      friend class FwdIter;
      friend class HashMap;

      FwdConstIter() = default;
      FwdConstIter(const FwdIter & it):
        parent_(it.parent_),
        pos_(it.pos_),
        first_(it.first_)
      {
        skip_empty();
      }
      FwdConstIter(const HashMap * parent, size_t pos, bool in_table1):
        parent_(parent),
        pos_(pos),
        first_(in_table1)
      {
        skip_empty();
      }

      reference operator*() const;
      pointer operator->() const;

      FwdConstIter & operator++();
      FwdConstIter operator++(int);

      bool operator!=(const FwdConstIter & rhs) const;
      bool operator==(const FwdConstIter & rhs) const;

      friend std::ostream & operator<<(std::ostream & os, const FwdIter & iter)
      {
        return os << iter->first;
      }
    private:
      const HashMap * parent_ = nullptr;
      size_t pos_ = 0;
      bool first_ = true;
      void skip_empty();
    };

    friend class FwdIter;
    friend class FwdConstIter;

    using iterator = FwdIter;
    using const_iterator = FwdConstIter;
    using val_type = std::pair< Key, T >;

    HashMap();
    HashMap(size_t size);
    template< class InputIterator >
    HashMap(InputIterator first, InputIterator last);
    HashMap(std::initializer_list< std::pair< Key, T > > il);

    size_t size() const;
    size_t capacity() const;
    bool empty() const noexcept;

    void clear() noexcept;
    void swap(HashMap & rhs);

    T & at(const Key & k);
    const T & at(const Key & k) const;

    T & operator[](const Key & k);
    T & operator[](Key && k );

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    iterator erase(const_iterator position);
    size_t erase(const Key & k);
    iterator erase(const_iterator fst, const_iterator last);

    template< class... Args >
    std::pair< iterator, bool > emplace(Args &&... args);
    template< class... Args >
    iterator emplace_hint(const_iterator hint, Args &&... args);

    std::pair< iterator, bool > insert(const val_type & val);
    iterator insert(const_iterator hint, const val_type & val);
    template< class InputIterator >
    void insert(InputIterator first, InputIterator last);

    iterator find(const Key & k);
    const_iterator find(const Key & k) const;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float z);

    void rehash(size_t n);

  private:

    std::vector< std::pair< std::pair< Key, T >, bool > > t1_;
    std::vector< std::pair< std::pair< Key, T >, bool > > t2_;

    size_t capacity_;
    size_t size_;

    static constexpr size_t MAX_ITERATIONS = 100;

    double max_load_factor_ = 0.75;

    T & insert_data(const Key & k);
  };

  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  HashMap< Key, T, HS1, HS2, EQ >::HashMap():
    HashMap(10)
  {}
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  HashMap< Key, T, HS1, HS2, EQ >::HashMap(size_t size):
    t1_(size),
    t2_(size),
    capacity_(size),
    size_(0)
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      t1_[i].second = false;
      t2_[i].second = false;
    }
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  template< class InputIterator >
  HashMap< Key, T, HS1, HS2, EQ >::HashMap(InputIterator first, InputIterator last):
    HashMap(10)
  {
    insert(first, last);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  HashMap< Key, T, HS1, HS2, EQ >::HashMap(std::initializer_list< std::pair< Key, T > > il):
    HashMap(il.begin(), il.end())
  {}
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  size_t HashMap< Key, T, HS1, HS2, EQ >::size() const
  {
    return size_;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  size_t HashMap< Key, T, HS1, HS2, EQ >::capacity() const
  {
    return capacity_;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  bool HashMap< Key, T, HS1, HS2, EQ >::empty() const noexcept
  {
    return size_ == 0;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  void HashMap< Key, T, HS1, HS2, EQ >::clear() noexcept
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (t1_[i].second)
      {
        t1_[i].second = false;
      }
      if (t2_[i].second)
      {
        t2_[i].second = false;
      }
    }

    size_ = 0;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  void HashMap< Key, T, HS1, HS2, EQ >::swap(HashMap< Key, T, HS1, HS2, EQ > & rhs)
  {
    t1_.swap(rhs.t1_);
    t2_.swap(rhs.t2_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_, rhs.size_);
    std::swap(max_load_factor_, rhs.max_load_factor_);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  T & HashMap< Key, T, HS1, HS2, EQ >::at(const Key & k)
  {
    auto data = find(k);
    if (data != end())
    {
      return data->second;
    }
    throw std::out_of_range("hashmap: at failed: no such item");
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  const T & HashMap< Key, T, HS1, HS2, EQ >::at(const Key & k) const
  {
    auto data = find(k);
    if (data != end())
    {
      return data->second;
    }
    throw std::out_of_range("hashmap: at failed: no such item");
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  T & HashMap< Key, T, HS1, HS2, EQ >::operator[](const Key & k)
  {
    auto data = find(k);
    if (data != end())
    {
      return data->second;
    }
    return insert_data(k);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  T & HashMap< Key, T, HS1, HS2, EQ >::operator[](Key && k)
  {
    auto data = find(k);
    if (data != end())
    {
      return data->second;
    }
    return insert_data(k);
  }
  template< typename K, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< K, T, HS1, HS2, EQ >::iterator HashMap< K, T, HS1, HS2, EQ >::begin() noexcept
  {
    return iterator(this, 0, true);
  }
  template< typename K, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< K, T, HS1, HS2, EQ >::const_iterator HashMap< K, T, HS1, HS2, EQ >::begin() const noexcept
  {
    return const_iterator(this, 0, true);
  }
  template< typename K, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< K, T, HS1, HS2, EQ >::const_iterator HashMap< K, T, HS1, HS2, EQ >::cbegin() const noexcept
  {
    return const_iterator(this, 0, true);
  }
  template< typename K, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< K, T, HS1, HS2, EQ >::iterator HashMap< K, T, HS1, HS2, EQ >::end() noexcept
  {
    return iterator(this, capacity_, false);
  }
  template< typename K, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< K, T, HS1, HS2, EQ >::const_iterator HashMap< K, T, HS1, HS2, EQ >::end() const noexcept
  {
    return const_iterator(this, capacity_, false);
  }
  template< typename K, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< K, T, HS1, HS2, EQ >::const_iterator HashMap< K, T, HS1, HS2, EQ >::cend() const noexcept
  {
    return const_iterator(this, capacity_, false);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  size_t HashMap< Key, T, HS1, HS2, EQ >::erase(const Key & k)
  {
    Key current_key = k;

    size_t h1 = HS1{}(current_key) % capacity_;
    if (t1_[h1].second)
    {
      Key old_key = t1_[h1].first.first;
      if (EQ{}(current_key, old_key))
      {
        t1_[h1].second = false;
        --size_;
        return 1ull;
      }
    }

    size_t h2 = HS2{}(current_key) % capacity_;
    if (t2_[h2].second)
    {
      Key old_key = t2_[h2].first.first;
      if (EQ{}(current_key, old_key))
      {
        t2_[h2].second = false;
        --size_;
        return 1ull;
      }
    }

    return 0ull;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< Key, T, HS1, HS2, EQ >::iterator HashMap< Key, T, HS1, HS2, EQ >::erase(const_iterator position)
  {
    iterator it = find(position->first);
    if (it != end())
    {
      if (it.first_)
      {
        t1_[it.pos_].second = false;
      }
      else
      {
        t2_[it.pos_].second = false;
      }
      --size_;
      ++it;
    }
    return it;
  }
  template< typename K, typename T, typename H, typename N, typename E >
  typename HashMap< K, T, H, N, E >::iterator HashMap< K, T, H, N, E >::erase(const_iterator fst, const_iterator last)
  {
    if (fst == cend())
    {
      return iterator(this, capacity_, false);
    }
    for (auto it = fst; it != last;)
    {
      it = erase(it);
    }
    return find(last->first);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  template< class... Args >
  std::pair
  <
    typename HashMap< Key, T, HS1, HS2, EQ >::iterator,
    bool
  >
  HashMap< Key, T, HS1, HS2, EQ >::emplace(Args &&... args)
  {
    std::pair< Key, T > temp(std::forward< Args >(args)...);

    auto it = find(temp.first);
    if (it != end())
    {
      return {it, false};
    }

    T & inserted = insert_data(temp.first);
    inserted = std::move(temp.second);
    return {find(temp.first), true};
  }
  template< typename K, typename T, typename A, typename B, typename E >
  template< class... X >
  typename HashMap< K, T, A, B, E >::iterator HashMap< K, T, A, B, E >::emplace_hint(const_iterator hint, X &&... args)
  {
    return emplace(std::forward< X >(args)...).first;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  std::pair
  <
    typename HashMap< Key, T, HS1, HS2, EQ >::iterator,
    bool
  >
  HashMap< Key, T, HS1, HS2, EQ >::insert(const val_type & val)
  {
    auto it = find(val.first);
    if (it != end())
    {
      return {it, false};
    }

    insert_data(val.first) = val.second;
    return {find(val.first), true};
  }
  template< typename K, typename T, typename A, typename B, typename E >
  typename HashMap< K, T, A, B, E >::iterator HashMap< K, T, A, B, E >::insert(const_iterator hint, const val_type & v)
  {
    return insert(v).first;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  template< class InputIterator >
  void HashMap< Key, T, HS1, HS2, EQ >::insert(InputIterator first, InputIterator last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< Key, T, HS1, HS2, EQ >::iterator HashMap< Key, T, HS1, HS2, EQ >::find(const Key & k)
  {
    size_t h1 = HS1{}(k) % capacity_;
    size_t h2 = HS2{}(k) % capacity_;

    if (t1_[h1].second && EQ{}(t1_[h1].first.first, k))
    {
      return iterator(this, h1, true);
    }

    if (t2_[h2].second && EQ{}(t2_[h2].first.first, k))
    {
      return iterator(this, h2, false);
    }

    return iterator(this, capacity_, false);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< Key, T, HS1, HS2, EQ >::const_iterator HashMap< Key, T, HS1, HS2, EQ >::find(const Key & k) const
  {
    size_t h1 = HS1{}(k) % capacity_;
    size_t h2 = HS2{}(k) % capacity_;

    if (t1_[h1].second && EQ{}(t1_[h1].first.first, k))
    {
      return const_iterator(this, h1, true);
    }

    if (t2_[h2].second && EQ{}(t2_[h2].first.first, k))
    {
      return const_iterator(this, h2, false);
    }

    return const_iterator(this, capacity_, false);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  float HashMap< Key, T, HS1, HS2, EQ >::load_factor() const noexcept
  {
    return static_cast< double >(size_) / capacity_;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  float HashMap< Key, T, HS1, HS2, EQ >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  void HashMap< Key, T, HS1, HS2, EQ >::max_load_factor(float z)
  {
    if (z > 0 && z <= 1)
    {
      max_load_factor_ = z;
    }
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  void HashMap< Key, T, HS1, HS2, EQ >::rehash(size_t n)
  {
    std::vector< std::pair< std::pair< Key, T >, bool > > new_t1(n);
    std::vector< std::pair< std::pair< Key, T >, bool > > new_t2(n);

    const size_t old_capacity = capacity_;
    const size_t old_size = size_;

    std::swap(t1_, new_t1);
    std::swap(t2_, new_t2);

    capacity_ = n;
    size_ = 0;

    try
    {
      for (size_t i = 0; i < old_capacity; ++i)
      {
        if (new_t1[i].second)
        {
          insert_data(new_t1[i].first.first) = new_t1[i].first.second;
        }
        if (new_t2[i].second)
        {
          insert_data(new_t2[i].first.first) = new_t2[i].first.second;
        }
      }
    }
    catch (...)
    {
      std::swap(t1_, new_t1);
      std::swap(t2_, new_t2);
      capacity_ = old_capacity;
      size_ = old_size;
      throw;
    }
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  T & HashMap< Key, T, HS1, HS2, EQ >::insert_data(const Key & k)
  {
    Key original_key = k;

    Key current_key = k;
    T current_value = T{};
    size_t attempts = 0;
    bool is_inserted = false;

    if (load_factor() > max_load_factor_)
    {
      rehash(capacity_ * 2);
    }

    while (attempts < MAX_ITERATIONS)
    {
      size_t h1 = HS1{}(current_key) % capacity_;
      if (!t1_[h1].second)
      {
        t1_[h1] = {{current_key, current_value}, true};
        size_++;
        is_inserted = true;
        break;
      }

      auto displaced = t1_[h1];
      t1_[h1] = {{current_key, current_value}, true};
      current_key = displaced.first.first;
      current_value = displaced.first.second;
      attempts++;

      size_t h2 = HS2{}(current_key) % capacity_;
      if (!t2_[h2].second)
      {
        t2_[h2] = {{current_key, current_value}, true};
        size_++;
        is_inserted = true;
        break;
      }

      displaced = t2_[h2];
      t2_[h2] = {{current_key, current_value}, true};
      current_key = displaced.first.first;
      current_value = displaced.first.second;
      attempts++;
    }

    if (!is_inserted)
    {
      rehash(capacity_ * 2);
      insert_data(current_key) = current_value;
    }

    return find(original_key)->second;
  }

  template< typename K, typename T, typename H1, typename H2, typename EQ >
  typename HashMap< K, T, H1, H2, EQ >::FwdIter::reference HashMap< K, T, H1, H2, EQ >::FwdIter::operator*()
  {
    return first_ ? parent_->t1_[pos_].first : parent_->t2_[pos_].first;
  }
  template< typename K, typename T, typename H1, typename H2, typename EQ >
  typename HashMap< K, T, H1, H2, EQ >::FwdIter::pointer HashMap< K, T, H1, H2, EQ >::FwdIter::operator->()
  {
    return std::addressof(operator*());
  }
  template< typename K, typename T, typename H1, typename H2, typename EQ >
  typename HashMap< K, T, H1, H2, EQ >::FwdIter::reference HashMap< K, T, H1, H2, EQ >::FwdIter::operator*() const
  {
    return first_ ? parent_->t1_[pos_].first : parent_->t2_[pos_].first;
  }
  template< typename K, typename T, typename H1, typename H2, typename EQ >
  typename HashMap< K, T, H1, H2, EQ >::FwdIter::pointer HashMap< K, T, H1, H2, EQ >::FwdIter::operator->() const
  {
    return std::addressof(operator*());
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< Key, T, HS1, HS2, EQ >::FwdIter & HashMap< Key, T, HS1, HS2, EQ >::FwdIter::operator++()
  {
    ++pos_;
    skip_empty();
    return *this;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< Key, T, HS1, HS2, EQ >::FwdIter HashMap< Key, T, HS1, HS2, EQ >::FwdIter::operator++(int)
  {
    FwdIter result(*this);
    ++(*this);
    return result;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  bool HashMap< Key, T, HS1, HS2, EQ >::FwdIter::operator!=(const FwdIter & rhs) const
  {
    return !(*this == rhs);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  bool HashMap< Key, T, HS1, HS2, EQ >::FwdIter::operator==(const FwdIter & rhs) const
  {
    if (parent_ != rhs.parent_)
    {
      return false;
    }
    bool c1 = parent_ && pos_ >= parent_->capacity_ && !first_;
    bool c2 = rhs.parent_ && rhs.pos_ >= rhs.parent_->capacity_ && !rhs.first_;
    if (c1 && c2)
    {
      return true;
    }
    return pos_ == rhs.pos_ && first_ == rhs.first_;
  }
  template< typename K, typename T, typename H1, typename H2, typename EQ >
  void HashMap< K, T, H1, H2, EQ >::FwdIter::skip_empty()
  {
    while (first_ && (pos_ < parent_->capacity_) && !parent_->t1_[pos_].second)
    {
      ++pos_;
    }
    if ((pos_ >= parent_->capacity_) && first_)
    {
      first_ = false;
      pos_ = 0;
    }
    while (!first_ && (pos_ < parent_->capacity_) && (!parent_->t2_[pos_].second))
    {
      ++pos_;
    }
  }

  template< typename K, typename T, typename A, typename B, typename E >
  typename HashMap< K, T, A, B, E >::FwdConstIter::reference HashMap< K, T, A, B, E >::FwdConstIter::operator*() const
  {
    return first_ ? parent_->t1_[pos_].first : parent_->t2_[pos_].first;
  }
  template< typename K, typename T, typename A, typename B, typename E >
  typename HashMap< K, T, A, B, E >::FwdConstIter::pointer HashMap< K, T, A, B, E >::FwdConstIter::operator->() const
  {
    return std::addressof(operator*());
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< Key, T, HS1, HS2, EQ >::FwdConstIter & HashMap< Key, T, HS1, HS2, EQ >::FwdConstIter::operator++()
  {
    ++pos_;
    skip_empty();
    return *this;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  typename HashMap< Key, T, HS1, HS2, EQ >::FwdConstIter HashMap< Key, T, HS1, HS2, EQ >::FwdConstIter::operator++(int)
  {
    FwdIter result(*this);
    ++(*this);
    return result;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  bool HashMap< Key, T, HS1, HS2, EQ >::FwdConstIter::operator!=(const FwdConstIter & rhs) const
  {
    return !(*this == rhs);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  bool HashMap< Key, T, HS1, HS2, EQ >::FwdConstIter::operator==(const FwdConstIter & rhs) const
  {
    if (parent_ != rhs.parent_)
    {
      return false;
    }
    bool c1 = parent_ && pos_ >= parent_->capacity_ && !first_;
    bool c2 = rhs.parent_ && rhs.pos_ >= rhs.parent_->capacity_ && !rhs.first_;
    if (c1 && c2)
    {
      return true;
    }
    return pos_ == rhs.pos_ && first_ == rhs.first_;
  }
  template< typename K, typename T, typename H1, typename H2, typename EQ >
  void HashMap< K, T, H1, H2, EQ >::FwdConstIter::skip_empty()
  {
    while (first_ && (pos_ < parent_->capacity_) && !parent_->t1_[pos_].second)
    {
      ++pos_;
    }
    if ((pos_ >= parent_->capacity_) && first_)
    {
      first_ = false;
      pos_ = 0;
    }
    while (!first_ && (pos_ < parent_->capacity_) && (!parent_->t2_[pos_].second))
    {
      ++pos_;
    }
  }
}

#endif
