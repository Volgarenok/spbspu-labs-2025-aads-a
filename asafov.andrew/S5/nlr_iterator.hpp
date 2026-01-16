#ifndef NLR_ITERATOR_HPP
#define NLR_ITERATOR_HPP
#include <map.hpp>
#include <stack.hpp>
#include <utility>

namespace asafov
{
  template< typename Key, typename Value >
  class nlr_iterator
  {
    friend class map< Key, Value >;

  public:
    nlr_iterator() = default;

    std::pair< Key, Value >& operator*() const
    {
      return stack_.top()->data;
    }

    std::pair< Key, Value >* operator->() const
    {
      return &(stack_.top()->data);
    }

    nlr_iterator& operator++()
    {
      map< Key, Value >::node* n = stack_.top();
      stack_.pop();
      if (n->right) stack_.push(n->right);
      if (n->left) stack_.push(n->left);
      return *this;
    }

    bool operator!=(const nlr_iterator& other) const
    {
      return stack_ != other.stack_;
    }

    bool operator==(const nlr_iterator& other) const
    {
      return stack_ == other.stack_;
    }

  private:
    asafov::Stack< map< Key, Value >::node* > stack_;

    explicit nlr_iterator(map< Key, Value >::node* root)
    {
      if (root)
        stack_.push(root);
    }
  };
}

#endif
