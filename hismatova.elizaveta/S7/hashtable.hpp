#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <functional>
#include <stdexcept>
#include <utility>
#include <string>

namespace hismatova
{

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
  private:
    enum EntryState
    {
      EMPTY,
      OCCUPIED,
      DELETED
    };

    struct Entry
    {
      std::pair< Key, Value > data;
      EntryState state;

      Entry():
        state(EMPTY)
      {}
      Entry(const Key& k, const Value& v):
        data(k, v),
        state(OCCUPIED)
      {}
    };

    std::vector< Entry > table_;
    size_t size_;
    size_t occupied_count_;
    Hash hasher_;
    Equal equal_;
    static const double MAX_LOAD_FACTOR;

    size_t probe(size_t hash, size_t i) const
    {
      return (hash + i * i) % table_.size();
    }

    size_t find_position(const Key& key) const
    {
      if (table_.empty())
      {
        return table_.size();
      }

      size_t hash = hasher_(key) % table_.size();
      size_t i = 0;
      size_t first_deleted = table_.size();

      while (i < table_.size())
      {
        size_t pos = probe(hash, i);
        const Entry& entry = table_[pos];

        if (entry.state == EMPTY)
        {
          break;
        }
        else if (entry.state == OCCUPIED && equal_(entry.data.first, key))
        {
          return pos;
        }
        else if (entry.state == DELETED && first_deleted == table_.size())
        {
          first_deleted = pos;
        }
        i++;
      }

      return first_deleted != table_.size() ? first_deleted: table_.size();
    }

    void rehash(size_t new_size)
    {
      std::vector< Entry > old_table = table_;
      table_.clear();
      table_.resize(new_size);
      size_ = 0;
      occupied_count_ = 0;

      for (const auto& entry: old_table)
      {
        if (entry.state == OCCUPIED)
        {
          insert(entry.data.first, entry.data.second);
        }
      }
    }

  public:
    HashTable():
      size_(0),
      occupied_count_(0)
    {
      table_.resize(11);
    }

    class const_iterator
    {
    private:
      const HashTable* table_;
      size_t pos_;

      void advance_to_occupied()
      {
        while (pos_ < table_->table_.size() &&
          table_->table_[pos_].state != OCCUPIED)
        {
          pos_++;
        }
      }

    public:
      const_iterator(const HashTable* table, size_t pos):
        table_(table),
        pos_(pos)
      {
        advance_to_occupied();
      }

      const std::pair< Key, Value >& operator*() const
      {
        return table_->table_[pos_].data;
      }

      const std::pair< Key, Value >* operator->() const
      {
        return &table_->table_[pos_].data;
      }

      const_iterator& operator++()
      {
        pos_++;
        advance_to_occupied();
        return *this;
      }

      const_iterator operator++(int)
      {
        const_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      bool operator==(const const_iterator& other) const
      {
        return table_ == other.table_ && pos_ == other.pos_;
      }

      bool operator!=(const const_iterator& other) const
      {
        return !(*this == other);
      }
    };

    const_iterator begin() const
    {
      return const_iterator(this, 0);
    }

    const_iterator end() const
    {
      return const_iterator(this, table_.size());
    }

    bool empty() const
    {
      return size_ == 0;
    }

    size_t size() const
    {
      return size_;
    }

    Value& at(const Key& key)
    {
      size_t pos = find_position(key);
      if (pos == table_.size() || table_[pos].state != OCCUPIED)
      {
        throw std::out_of_range("Key not found");
      }
      return table_[pos].data.second;
    }

    const Value& at(const Key& key) const
    {
      size_t pos = find_position(key);
      if (pos == table_.size() || table_[pos].state != OCCUPIED)
      {
        throw std::out_of_range("Key not found");
      }
      return table_[pos].data.second;
    }

    Value& operator[](const Key& key)
    {
      size_t pos = find_position(key);
      if (pos == table_.size() || table_[pos].state != OCCUPIED)
      {
        insert(key, Value());
        pos = find_position(key);
      }
      return table_[pos].data.second;
    }

    std::pair< const_iterator, bool > insert(const Key& key, const Value& value)
    {
      if (load_factor() > MAX_LOAD_FACTOR)
      {
        rehash(table_.size() * 2 + 1);
      }

      size_t pos = find_position(key);
      if (pos != table_.size() && table_[pos].state == OCCUPIED)
      {
        return std::make_pair(const_iterator(this, pos), false);
      }

      if (pos == table_.size())
      {
        size_t hash = hasher_(key) % table_.size();
        size_t i = 0;
        while (i < table_.size())
        {
          pos = probe(hash, i);
          if (table_[pos].state != OCCUPIED)
          {
            break;
          }
          i++;
        }
      }

      table_[pos] = Entry(key, value);
      size_++;
      if (table_[pos].state != OCCUPIED)
      {
        occupied_count_++;
      }
      table_[pos].state = OCCUPIED;

      return std::make_pair(const_iterator(this, pos), true);
    }

    size_t erase(const Key& key)
    {
      size_t pos = find_position(key);
      if (pos == table_.size() || table_[pos].state != OCCUPIED)
      {
        return 0;
      }

      table_[pos].state = DELETED;
      size_--;
      return 1;
    }

    void clear()
    {
      for (size_t i = 0; i < table_.size(); i++)
      {
        table_[i].state = EMPTY;
      }
      size_ = 0;
      occupied_count_ = 0;
    }

    const_iterator find(const Key& key) const
    {
      size_t pos = find_position(key);
      if (pos == table_.size() || table_[pos].state != OCCUPIED)
      {
        return end();
      }
      return const_iterator(this, pos);
    }

    bool contains(const Key& key) const
    {
      return find(key) != end();
    }

    float load_factor() const
    {
      return table_.empty() ? 0.0f: static_cast< float >(occupied_count_) / table_.size();
    }

    void rehash_policy(size_t count)
    {
      if (count >= size_ / MAX_LOAD_FACTOR)
      {
        rehash(count);
      }
    }
  };

}

#endif
