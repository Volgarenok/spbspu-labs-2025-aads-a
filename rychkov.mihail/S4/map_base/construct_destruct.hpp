#ifndef MAP_BASE_CONSTRUCT_DESTRUCT_HPP
#define MAP_BASE_CONSTRUCT_DESTRUCT_HPP

#include "declaration.hpp"

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::MapBase()
    noexcept(std::is_nothrow_default_constructible< value_compare >::value):
  comp_(),
  cached_begin_(fake_root()),
  cached_rbegin_(fake_root()),
  size_(0),
  fake_parent_(nullptr),
  fake_children_{nullptr},
  fake_size_(0)
{}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::MapBase(value_compare key)
    noexcept(std::is_nothrow_move_constructible< value_compare >::value):
  comp_(std::move(key)),
  cached_begin_(fake_root()),
  cached_rbegin_(fake_root()),
  size_(0),
  fake_parent_(nullptr),
  fake_children_{nullptr},
  fake_size_(0)
{}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::MapBase(MapBase&& rhs)
    noexcept(std::is_nothrow_move_constructible< value_compare >::value):
  comp_(std::move(rhs.comp_)),
  cached_begin_(std::exchange(rhs.cached_begin_, rhs.fake_root())),
  cached_rbegin_(std::exchange(rhs.cached_rbegin_, rhs.fake_root())),
  size_(std::exchange(rhs.size_, 0)),
  fake_parent_(nullptr),
  fake_children_{std::exchange(rhs.fake_children_[0], nullptr)},
  fake_size_(0)
{
  if (size_ == 0)
  {
    cached_rbegin_ = cached_begin_ = fake_root();
  }
  else
  {
    fake_children_[0]->parent = fake_root();
  }
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::MapBase(const MapBase& rhs):
  MapBase(rhs.begin(), rhs.end(), rhs.comp_)
{}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::MapBase
    (std::initializer_list< value_type > init, value_compare compare):
  MapBase(init.begin(), init.end(), std::move(compare))
{}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class InputIt >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::MapBase(InputIt from, InputIt to, value_compare compare):
  MapBase(std::move(compare))
{
  for (; from != to; ++from)
  {
    insert(*from);
  }
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >&
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::operator=(const MapBase& rhs)
{
  MapBase temp = rhs;
  swap(temp);
  return *this;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >&
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::operator=(MapBase&& rhs)
    noexcept(noexcept(swap(std::declval< MapBase& >())))
{
  MapBase temp = std::move(rhs);
  swap(temp);
  return *this;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >&
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::operator=(std::initializer_list< value_type > init)
{
  MapBase temp = init;
  swap(temp);
  return *this;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::~MapBase()
{
  clear();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
void rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::swap(MapBase& rhs)
    noexcept(is_nothrow_swappable_v< value_compare >)
{
  std::swap(comp_, rhs.comp_);
  std::swap(cached_begin_, rhs.cached_begin_);
  std::swap(cached_rbegin_, rhs.cached_rbegin_);
  std::swap(size_, rhs.size_);
  std::swap(fake_children_[0], rhs.fake_children_[0]);
  if (size_ == 0)
  {
    cached_rbegin_ = cached_begin_ = fake_root();
  }
  else
  {
    fake_children_[0]->parent = fake_root();
  }
  if (rhs.size_ == 0)
  {
    rhs.cached_rbegin_ = rhs.cached_begin_ = fake_root();
  }
  else
  {
    rhs.fake_children_[0]->parent = rhs.fake_root();
  }
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
void rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::clear()
{
  destroy_subtree(fake_children_[0]);
  delete fake_children_[0];
  fake_children_[0] = nullptr;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
void rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::destroy_subtree(node_type* node)
{
  if (node == nullptr)
  {
    return;
  }
  for (node_size_type i = 0; i <= node->size(); i++)
  {
    node_type* child = node->children[i];
    destroy_subtree(child);
    delete child;
  }
}

#endif
