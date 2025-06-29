#ifndef MAP_BASE_NODE_HPP
#define MAP_BASE_NODE_HPP

#include <cstddef>
#include <utility>
#include <memory>
#include <type_tools.hpp>

namespace rychkov
{
  template< class Value, size_t N >
  class MapBaseNode
  {
  public:
    using value_type = Value;
    using size_type = select_size_type_t< N >;
    static constexpr size_t node_capacity = N;

    static_assert(N >= 2, "");

    MapBaseNode* parent = nullptr;
    MapBaseNode* children[node_capacity + 1];

    ~MapBaseNode()
    {
      while (!empty())
      {
        pop_back();
      }
    }
    value_type& operator[](size_type i)
    {
      return *(reinterpret_cast< value_type* >(data_) + i);
    }
    const value_type& operator[](size_type i) const
    {
      return *(reinterpret_cast< value_type* >(data_) + i);
    }
    bool empty() const noexcept
    {
      return size_ == 0;
    }
    bool full() const noexcept
    {
      return size_ >= node_capacity;
    }
    size_type size() const noexcept
    {
      return size_;
    }
    bool isfake() const noexcept
    {
      return parent == nullptr;
    }
    bool isleaf() const noexcept
    {
      return children[0] == nullptr;
    }

    template< class... Args >
    void emplace(size_type newPlace, Args&&... args)
    {
      for (size_type i = size_; i > newPlace; i--)
      {
        new(std::addressof(operator[](i))) value_type{std::move(operator[](i - 1))};
        operator[](i - 1).~value_type();
        children[i + 1] = children[i];
      }
      new(std::addressof(operator[](newPlace))) value_type{std::forward< Args >(args)...};
      if (newPlace == 0)
      {
        children[1] = (size_ == 0 ? nullptr : children[0]);
        children[0] = nullptr;
      }
      else
      {
        children[newPlace + 1] = nullptr;
      }
      size_++;
    }
    template< class... Args >
    void emplace_back(Args&&... args)
    {
      emplace(size_, std::forward< Args >(args)...);
    }
    template< class... Args >
    void replace(size_type i, Args&&... args)
    {
      operator[](i).~value_type();
      new(std::addressof(operator[](i))) value_type{std::forward< Args >(args)...};
    }
    void erase(size_type i)
    {
      operator[](i).~value_type();
      for (size_type j = i + 1; j < size_; j++)
      {
        new(std::addressof(operator[](j - 1))) value_type{std::move(operator[](j))};
        operator[](j).~value_type();
        children[j] = children[j + 1];
      }
      size_--;
    }
    void pop_back()
    {
      operator[](--size_).~value_type();
    }
  private:
    size_type size_ = 0;
    alignas(value_type) unsigned char data_[node_capacity * sizeof(value_type)];
  };
}

#endif
