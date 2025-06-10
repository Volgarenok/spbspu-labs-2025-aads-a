#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP
#include <dynamic-array.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/xxhash.hpp>
#include "boost-hash-wrapper.hpp"

namespace savintsev
{
  template
  <
    typename Key,
    typename T,
    typename HS1 = BoostHash< Key, boost::hash2::fnv1a_64 >,
    typename HS2 = BoostHash< Key, boost::hash2::xxhash_64 >,
    typename EQ = std::equal_to<>
  >
  class HashMap
  {
  private:

    Array< std::pair< std::pair< Key, T >, bool > > t1_;
    Array< std::pair< std::pair< Key, T >, bool > > t2_;

    size_t capacity_;
    size_t size_ = 0;

    static constexpr size_t MAX_ITERATIONS = 100;
    
    double max_load_factor_ = 0.75;

    T & insert_data(const Key & k);
    void delete_data(const Key & k);
    bool lookup_data(const Key & k);
    std::pair< T &, bool > get_data(const Key & k);

    void automatic_rehash();

  public:

    HashMap();
    HashMap(size_t size);

    size_t size() const;
    bool empty() const noexcept;

    void clear() noexcept;
    void swap(HashMap & rhs);

    T & at(const Key & k);
    const T & at(const Key & k) const;

    T & operator[](const Key & k);
    T & operator[](Key && k );

    //iterator erase ( const_iterator position );
    size_t erase(const Key & k);
    //iterator erase ( const_iterator first, const_iterator last );

    //std::pair< iterator, bool > insert(const T & val );

    //iterator find ( const key_type& k );
    //const_iterator find ( const key_type& k ) const;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float z);

    void rehash(size_t n);

  };

  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  HashMap< Key, T, HS1, HS2, EQ >::HashMap():
    HashMap(10)
  {}
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  HashMap< Key, T, HS1, HS2, EQ >::HashMap(size_t size):
    t1_(size),
    t2_(size),
    capacity_(size)
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      t1_[i].second = true;
      t2_[i].second = true;
    }
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  size_t HashMap< Key, T, HS1, HS2, EQ >::size() const
  {
    return size_;
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
      if (!t1_[i].second)
      {
        using pair_type = decltype(t1_[i].first);
        t1_[i].first.~pair_type();
        t1_[i].second = true;
      }
      if (!t2_[i].second)
      {
        using pair_type = decltype(t2_[i].first);
        t2_[i].first.~pair_type();
        t2_[i].second = true;
      }
    }

    size_ = 0;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  void HashMap< Key, T, HS1, HS2, EQ >::swap(HashMap & rhs)
  {
    swap(t1_, rhs.t1_);
    swap(t2_, rhs.t2_);
    swap(capacity_, rhs.capacity_);
    swap(size_, rhs.size_);
    swap(max_load_factor_, rhs.max_load_factor_);
  }
  template <typename Key, typename T, typename HS1, typename HS2, typename EQ>
  T &HashMap<Key, T, HS1, HS2, EQ>::at(const Key &k)
  {
    auto data = get_data(k);
    if (data.second)
    {
      return data.first;
    }
    throw std::runtime_error("hashmap: at failed: no such item");
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  const T & HashMap< Key, T, HS1, HS2, EQ >::at(const Key & k) const
  {
    auto data = get_data(k);
    if (data.second)
    {
      return data.first;
    }
    throw std::runtime_error("hashmap: at failed: no such item");
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  T & HashMap< Key, T, HS1, HS2, EQ >::operator[](const Key & k)
  {
    auto data = get_data(k);
    if (data.second)
    {
      return data.first;
    }
    automatic_rehash();
    return insert_data(k);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  T & HashMap< Key, T, HS1, HS2, EQ >::operator[](Key && k)
  {
    auto data = get_data(k);
    if (data.second)
    {
      return data.first;
    }
    automatic_rehash();
    return insert_data(k);
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  float HashMap< Key, T, HS1, HS2, EQ >::load_factor() const noexcept
  {
    double factor = size_ / (capacity_ * 2);
    return factor;
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
    HashMap< Key, T, HS1, HS2, EQ > map(n);

    map.max_load_factor_ = max_load_factor_;

    for (size_t i = 0; i < capacity_; ++i)
    {
      if (!t1_[i].second)
      {
        map.insert_data(t1_[i].first.first) = t1_[i].first.second;
      }
      if (!t2_[i].second)
      {
        map.insert_data(t2_[i].first.first) = t2_[i].first.second;
      }
    }
    t1_ = std::move(map.t1_);
    t2_ = std::move(map.t2_);
    capacity_ = map.capacity_;
    size_ = map.size_;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  T & HashMap< Key, T, HS1, HS2, EQ >::insert_data(const Key & k)
  {
    Key current_key = k;
    T current_t = T{};

    for (size_t i = 0; i < MAX_ITERATIONS; ++i)
    {
      size_t h1 = HS1{}(current_key) % capacity_;
      if (t1_[h1].second)
      {
        t1_[h1] = {{current_key, current_t}, false};
        size_++;
        return t1_[h1].first.second;
      }

      auto displaced = t1_[h1];
      t1_[h1] = {{current_key, current_t}, false};
      current_key = displaced.first.first;
      current_t = displaced.first.second;

      size_t h2 = HS2{}(current_key) % capacity_;
      if (t2_[h2].second)
      {
        t2_[h2] = {{current_key, current_t}, false};
        size_++;
        return t2_[h2].first.second;;
      }

      displaced = t2_[h2];
      t2_[h2] = {{current_key, current_t}, false};
      current_key = displaced.first.first;
      current_t = displaced.first.second;
    }

    throw std::runtime_error("hashmap: cuckoo failed: iteration limit exceeded");
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  void HashMap< Key, T, HS1, HS2, EQ >::delete_data(const Key & k)
  {
    Key current_key = k;

    size_t h1 = HS1{}(current_key) % capacity_;
    if (!t1_[h1].second)
    {
      Key old_key = t1_[h1].first.first;
      if (EQ{}(current_key, old_key))
      {
        t1_[h1].second = true;
        --size_;
        return;
      }
    }

    size_t h2 = HS2{}(current_key) % capacity_;
    if (!t2_[h2].second)
    {
      Key old_key = t2_[h2].first.first;
      if (EQ{}(current_key, old_key))
      {
        t2_[h2].second = true;
        --size_;
        return;
      }
    }
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  bool HashMap< Key, T, HS1, HS2, EQ >::lookup_data(const Key & k)
  {
    return get_data(k).second;
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  std::pair< T &, bool > HashMap< Key, T, HS1, HS2, EQ >::get_data(const Key & k)
  {
    Key current_key = k;

    size_t h1 = HS1{}(current_key) % capacity_;
    if (!t1_[h1].second)
    {
      Key old_key = t1_[h1].first.first;
      if (EQ{}(current_key, old_key))
      {
        return {t1_[h1].first.second, true};
      }
    }

    size_t h2 = HS2{}(current_key) % capacity_;
    if (!t2_[h2].second)
    {
      Key old_key = t2_[h2].first.first;
      if (EQ{}(current_key, old_key))
      {
        return {t2_[h2].first.second, true};
      }
    }
    return {t1_[0].first.second, false};
  }
  template< typename Key, typename T, typename HS1, typename HS2, typename EQ >
  void HashMap< Key, T, HS1, HS2, EQ >::automatic_rehash()
  {
    if (max_load_factor_ < load_factor())
    {
      rehash(capacity_ + 5);
    }
  }
}

#endif
