#ifndef BITREENODE_HPP
#define BITREENODE_HPP
#include <utility>
#include <cstddef>
namespace nikonov
{
  namespace detail
  {
    template< typename Key, typename Value >
    class BiTreeNode
    {
    public:
      std::pair< Key, Value > data_;
      BiTreeNode* parent_;
      BiTreeNode* left_;
      BiTreeNode* right_;
      BiTreeNode(const Key& key, const Value& val);
    };
    template< typename Key, typename Value >
    BiTreeNode< Key, Value >::BiTreeNode(const Key& key, const Value& val):
      data_(std::make_pair(key, val)),
      parent_(nullptr),
      left_(nullptr),
      right_(nullptr)
    {}
  }
}
#endif
