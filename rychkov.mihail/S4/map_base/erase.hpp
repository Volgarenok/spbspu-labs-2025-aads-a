#ifndef MAP_BASE_ERASE_HPP
#define MAP_BASE_ERASE_HPP

#include "declaration.hpp"

#include <iterator>
#include <type_traits.hpp>

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::erase(const_iterator pos)
{
  static_assert(std::is_nothrow_move_constructible< real_value_type >::value, "move construct need to be nothrow");

  size_--;
  iterator result{pos.node_, pos.pointed_};
  ++result;
  if (!pos.node_->isleaf())
  {
    const_iterator erased = pos;
    --pos;
    erased.node_->replace(erased.pointed_, std::move(*pos));
  }
  if (pos.node_->size() > 1)
  {
    pos.node_->erase(pos.pointed_);
    correct_erase_result(pos, pos, result, false);
    return result;
  }
  while (true)
  {
    if (pos.node_->parent == fake_root())
    {
      if (pos.node_->isleaf())
      {
        fake_children_[0] = nullptr;
        cached_begin_ = fake_root();
        cached_rbegin_ = cached_begin_;
        delete pos.node_;
        return result;
      }
      fake_children_[0] = pos.node_->children[0];
      fake_children_[0]->parent = fake_root();
      delete pos.node_;
      return result;
    }
    pos.node_->pop_back();
    const_iterator erased = pos;
    pos.move_up();
    const_iterator parent = pos;
    parent.pointed_ -= (parent.pointed_ == 0 ? 0 : 1);
    node_type* src = nullptr;
    node_type* erased_subtree = erased.node_->children[0];
    if (pos.pointed_ != 0)
    {
      src = parent.node_->children[parent.pointed_];
      if (src->size() != 1)
      {
        erased.node_->emplace_back(std::move(parent.node_->operator[](parent.pointed_)));
        parent.node_->replace(parent.pointed_, std::move(src->operator[](src->size() - 1)));
        correct_erase_result(erased, parent, result, true);
        correct_erase_result(parent, const_iterator(src, src->size() - 1), result, false);
        erased.node_->children[1] = erased_subtree;
        erased.node_->children[0] = src->children[src->size()];
        if (!src->isleaf())
        {
          erased.node_->children[0]->parent = erased.node_;
        }
        src->pop_back();
        return result;
      }
      else
      {
        erased.node_->emplace_back(std::move(src->operator[](0)));
        erased.node_->emplace_back(std::move(parent.node_->operator[](parent.pointed_)));
        correct_erase_result(erased, {src, 0}, result, false);
        correct_erase_result({erased.node_, 1}, parent, result, false);
        erased.node_->children[2] = erased_subtree;
        erased.node_->children[1] = src->children[1];
        erased.node_->children[0] = src->children[0];
        if (!src->isleaf())
        {
          erased.node_->children[1]->parent = erased.node_;
          erased.node_->children[0]->parent = erased.node_;
        }
        src->pop_back();
        if (src == cached_begin_)
        {
          cached_begin_ = erased.node_;
        }
        delete src;
        parent.node_->children[parent.pointed_] = erased.node_;
        pos = parent;
      }
    }
    else
    {
      src = parent.node_->children[pos.pointed_ + 1];
      if (src->size() != 1)
      {
        erased.node_->emplace_back(std::move(parent.node_->operator[](pos.pointed_)));
        parent.node_->replace(pos.pointed_, std::move(src->operator[](0)));
        correct_erase_result(erased, pos, result, true);
        correct_erase_result(pos, {src, 0}, result, false);
        erased.node_->children[0] = erased_subtree;
        erased.node_->children[1] = src->children[0];
        if (!src->isleaf())
        {
          erased.node_->children[1]->parent = erased.node_;
        }
        src->children[0] = src->children[1];
        src->erase(0);
        return result;
      }
      else
      {
        erased.node_->emplace_back(std::move(parent.node_->operator[](parent.pointed_)));
        erased.node_->emplace_back(std::move(src->operator[](0)));
        correct_erase_result(erased, parent, result, false);
        correct_erase_result({erased.node_, 1}, {src, 0}, result, false);
        erased.node_->children[0] = erased_subtree;
        erased.node_->children[1] = src->children[0];
        erased.node_->children[2] = src->children[1];
        if (!src->isleaf())
        {
          erased.node_->children[1]->parent = erased.node_;
          erased.node_->children[2]->parent = erased.node_;
        }
        src->pop_back();
        if (src == cached_begin_)
        {
          cached_begin_ = erased.node_;
        }
        delete src;
        parent.node_->children[parent.pointed_] = erased.node_;
      }
    }
    if (parent.node_->size() > 1)
    {
      parent.node_->erase(pos.pointed_);
      correct_erase_result(pos, pos, result, false);
      return result;
    }
  }
  return result;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
void rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::correct_erase_result(const_iterator to,
    const_iterator from, iterator& result, bool will_be_replaced)
{
  if ((from.node_ != result.node_) || (result.pointed_ < from.pointed_))
  {
    return;
  }
  if (from.pointed_ == result.pointed_)
  {
    result = {to.node_, to.pointed_};
    return;
  }
  if (!will_be_replaced)
  {
    result.pointed_--;
  }
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::size_type
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::erase(const key_type& key)
{
  size_type result = 0;
  const_iterator iter = lower_bound(key);
  for (; (iter != end()) && !compare_with_key(key, *iter); iter = erase(iter), result++)
  {}
  return result;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::size_type, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::erase(const K1& key)
{
  size_type result = 0;
  const_iterator iter = lower_bound(key);
  for (; (iter != end()) && !compare_with_key(key, *iter); iter = erase(iter), result++)
  {}
  return result;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::erase(const_iterator from, const_iterator to)
{

  for (size_type len = std::distance(from, to); len > 0; len--)
  {
    from = erase(from);
  }
  return {from.node_, from.pointed_};
}

#endif
