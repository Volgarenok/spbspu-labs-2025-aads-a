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
      std::pair< Key, Value > data;
      BiTreeNode* parent;
      BiTreeNode* left;
      BiTreeNode* right;
      BiTreeNode(const Key& key, const Value& val);
    };
    template< typename Key, typename Value >
    BiTreeNode< Key, Value >::BiTreeNode(const Key& key, const Value& val):
      data(std::make_pair(key, val)),
      parent(nullptr),
      left(nullptr),
      right(nullptr)
    {}
  }
}
#endif
