#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H
#include <vector>
#include <list>
#include <initializer_list>
#include <utility>
#include <functional>
#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace asafov
{
  template< class Key, class T, class Hash = std::hash< Key >, class KeyEqual = std::equal_to< Key > >
  class UnorderedMap
  {
  public:
    UnorderedMap()
    {
      buckets_.resize(default_bucket_count_);
    }

    UnorderedMap(std::initializer_list< std::pair< const Key, T > > list)
    {
      buckets_.resize(default_bucket_count_);
      insert(list.begin(), list.end());
    }

    template< typename InputIt >
    UnorderedMap(InputIt first, InputIt last)
    {
      buckets_.resize(default_bucket_count_);
      insert(first, last);
    }

    UnorderedMap(const UnorderedMap& other):
      buckets_(other.buckets_),
      size_(other.size_),
      max_load_factor_(other.max_load_factor_),
      hash_(other.hash_),
      key_equal_(other.key_equal_)
    {
    }

    UnorderedMap(UnorderedMap&& other) noexcept:
      buckets_(std::move(other.buckets_)),
      size_(other.size_),
      max_load_factor_(other.max_load_factor_),
      hash_(std::move(other.hash_)),
      key_equal_(std::move(other.key_equal_))
    {
      other.size_ = 0;
    }

    UnorderedMap& operator=(const UnorderedMap& other)
    {
      if (this != &other)
      {
        buckets_ = other.buckets_;
        size_ = other.size_;
        max_load_factor_ = other.max_load_factor_;
        hash_ = other.hash_;
        key_equal_ = other.key_equal_;
      }
      return *this;
    }

    UnorderedMap& operator=(UnorderedMap&& other) noexcept
    {
      if (this != &other)
      {
        buckets_ = std::move(other.buckets_);
        size_ = other.size_;
        max_load_factor_ = other.max_load_factor_;
        hash_ = std::move(other.hash_);
        key_equal_ = std::move(other.key_equal_);
        other.size_ = 0;
      }
      return *this;
    }

    class Iterator
    {
      friend class UnorderedMap;
      using bucket_list = std::vector< std::list< std::pair< const Key, T > > >;
      bucket_list* buckets_;
      size_t bucket_index_;
      typename std::list< std::pair< const Key, T > >::iterator list_iterator_;

      void advance()
      {
        while (bucket_index_ < buckets_->size())
        {
          auto& bucket = (*buckets_)[bucket_index_];
          if (list_iterator_ != bucket.end())
            return;
          ++bucket_index_;
          if (bucket_index_ < buckets_->size())
            list_iterator_ = (*buckets_)[bucket_index_].begin();
        }
      }

      Iterator(bucket_list& buckets, bool is_end)
        : buckets_(&buckets), bucket_index_(0)
      {
        if (is_end)
        {
          bucket_index_ = buckets.size();
        }
        else
        {
          list_iterator_ = (*buckets_)[0].begin();
          advance();
        }
      }

      Iterator(bucket_list& buckets, size_t index, typename std::list< std::pair< const Key, T > >::iterator it)
        : buckets_(&buckets), bucket_index_(index), list_iterator_(it)
      {
      }

    public:
      Iterator& operator++()
      {
        ++list_iterator_;
        advance();
        return *this;
      }

      Iterator operator++(int)
      {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      std::pair< const Key, T >& operator*()
      {
        return *list_iterator_;
      }

      std::pair< const Key, T >* operator->()
      {
        return &(*list_iterator_);
      }

      bool operator==(const Iterator& other) const
      {
        return buckets_ == other.buckets_ && bucket_index_ == other.bucket_index_ &&
          (bucket_index_ == buckets_->size() || list_iterator_ == other.list_iterator_);
      }

      bool operator!=(const Iterator& other) const
      {
        return !(*this == other);
      }
    };

    class ConstIterator
    {
      friend class UnorderedMap;
      using bucket_list = const std::vector< std::list< std::pair< const Key, T > > >;
      const bucket_list* buckets_;
      size_t bucket_index_;
      typename std::list< std::pair< const Key, T > >::const_iterator list_iterator_;

      void advance()
      {
        while (bucket_index_ < buckets_->size())
        {
          auto& bucket = (*buckets_)[bucket_index_];
          if (list_iterator_ != bucket.end())
            return;
          ++bucket_index_;
          if (bucket_index_ < buckets_->size())
            list_iterator_ = (*buckets_)[bucket_index_].begin();
        }
      }

      ConstIterator(const bucket_list& buckets, bool is_end)
        : buckets_(&buckets), bucket_index_(0)
      {
        if (is_end)
        {
          bucket_index_ = buckets.size();
        }
        else
        {
          list_iterator_ = buckets[0].begin();
          advance();
        }
      }

      ConstIterator(const bucket_list& buckets, size_t index,
                    typename std::list< std::pair< const Key, T > >::const_iterator it)
        : buckets_(&buckets), bucket_index_(index), list_iterator_(it)
      {
      }

    public:
      ConstIterator& operator++()
      {
        ++list_iterator_;
        advance();
        return *this;
      }

      ConstIterator operator++(int)
      {
        ConstIterator tmp = *this;
        ++(*this);
        return tmp;
      }

      const std::pair< const Key, T >& operator*() const
      {
        return *list_iterator_;
      }

      const std::pair< const Key, T >* operator->() const
      {
        return &(*list_iterator_);
      }

      bool operator==(const ConstIterator& other) const
      {
        return buckets_ == other.buckets_ && bucket_index_ == other.bucket_index_ &&
          (bucket_index_ == buckets_->size() || list_iterator_ == other.list_iterator_);
      }

      bool operator!=(const ConstIterator& other) const
      {
        return !(*this == other);
      }
    };

    Iterator begin()
    {
      return Iterator(buckets_, false);
    }

    ConstIterator begin() const
    {
      return cbegin();
    }

    ConstIterator cbegin() const
    {
      return ConstIterator(buckets_, false);
    }

    Iterator end()
    {
      return Iterator(buckets_, true);
    }

    ConstIterator end() const
    {
      return cend();
    }

    ConstIterator cend() const
    {
      return ConstIterator(buckets_, true);
    }

    bool empty() const
    {
      return size_ == 0;
    }

    size_t size() const
    {
      return size_;
    }

    float load_factor() const
    {
      return static_cast< float >(size_) / buckets_.size();
    }

    float max_load_factor() const
    {
      return max_load_factor_;
    }

    void max_load_factor(float ml)
    {
      max_load_factor_ = ml;
      if (load_factor() > max_load_factor_)
      {
        rehash(buckets_.size() * 2);
      }
    }

    void rehash(size_t count)
    {
      std::vector< std::list< std::pair< const Key, T > > > new_buckets(count);
      for (const auto& bucket: buckets_)
      {
        for (const auto& pair: bucket)
        {
          size_t index = hash_(pair.first) % count;
          new_buckets[index].push_back(pair);
        }
      }
      buckets_ = std::move(new_buckets);
    }

    void clear()
    {
      for (auto& bucket: buckets_)
      {
        bucket.clear();
      }
      size_ = 0;
    }

    void swap(UnorderedMap& other)
    {
      std::swap(buckets_, other.buckets_);
      std::swap(size_, other.size_);
      std::swap(max_load_factor_, other.max_load_factor_);
      std::swap(hash_, other.hash_);
      std::swap(key_equal_, other.key_equal_);
    }

    std::pair< Iterator, bool > insert(const std::pair< const Key, T >& value)
    {
      ensure_capacity();
      size_t index = bucket_index(value.first);
      auto& bucket = buckets_[index];

      for (auto it = bucket.begin(); it != bucket.end(); ++it)
      {
        if (key_equal_(it->first, value.first))
        {
          return {Iterator(buckets_, index, it), false};
        }
      }

      bucket.push_back(value);
      ++size_;
      return {Iterator(buckets_, index, std::prev(bucket.end())), true};
    }

    Iterator insert(Iterator hint, const std::pair< const Key, T >& value)
    {
      (void)hint;
      return insert(value).first;
    }

    template< typename InputIt >
    void insert(InputIt first, InputIt last)
    {
      for (; first != last; ++first)
      {
        insert(*first);
      }
    }

    size_t erase(const Key& key)
    {
      size_t index = bucket_index(key);
      auto& bucket = buckets_[index];

      for (auto it = bucket.begin(); it != bucket.end(); ++it)
      {
        if (key_equal_(it->first, key))
        {
          bucket.erase(it);
          --size_;
          return 1;
        }
      }

      return 0;
    }

    Iterator erase(Iterator pos)
    {
      if (pos == end()) return end();
      auto& bucket = buckets_[pos.bucket_index_];
      auto next = std::next(pos.list_iterator_);
      bucket.erase(pos.list_iterator_);
      --size_;
      if (next != bucket.end())
      {
        return Iterator(buckets_, pos.bucket_index_, next);
      }
      return ++Iterator(buckets_, pos.bucket_index_, bucket.end());
    }

    Iterator erase(Iterator first, Iterator last)
    {
      while (first != last)
      {
        first = erase(first);
      }
      return last;
    }

    Iterator find(const Key& key)
    {
      size_t index = bucket_index(key);
      auto& bucket = buckets_[index];
      for (auto it = bucket.begin(); it != bucket.end(); ++it)
      {
        if (key_equal_(it->first, key))
        {
          return Iterator(buckets_, index, it);
        }
      }
      return end();
    }

    ConstIterator find(const Key& key) const
    {
      size_t index = bucket_index(key);
      auto& bucket = buckets_[index];
      for (auto it = bucket.begin(); it != bucket.end(); ++it)
      {
        if (key_equal_(it->first, key))
        {
          return ConstIterator(buckets_, index, it);
        }
      }
      return cend();
    }

  private:
    size_t bucket_index(const Key& key) const
    {
      return hash_(key) % buckets_.size();
    }

    void ensure_capacity()
    {
      if (load_factor() > max_load_factor_)
      {
        rehash(buckets_.size() * 2);
      }
    }

    static constexpr float default_max_load_factor_ = 1.0f;
    static constexpr size_t default_bucket_count_ = 8;

    std::vector< std::list< std::pair< const Key, T > > > buckets_;
    size_t size_ = 0;
    float max_load_factor_ = default_max_load_factor_;

    Hash hash_;
    KeyEqual key_equal_;
  };
}
#endif
