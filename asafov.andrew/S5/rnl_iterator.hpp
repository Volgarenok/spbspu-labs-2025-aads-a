#ifndef DETOURS_HPP
#define DETOURS_HPP
#include <map.hpp>
#include <stack.hpp>
#include <utility>

namespace asafov
{
  template< typename Key, typename Value >
  class rnl_iterator
  {
    friend class map< Key, Value >;

  public:
    rnl_iterator() = default;

    std::pair< Key, Value >& operator*() const
    {
      return stack_.top()->data;
    }

    std::pair< Key, Value >* operator->() const
    {
      return &(stack_.top()->data);
    }

    rnl_iterator& operator++()
    {
      map< Key, Value >::node* n = stack_.top();
      stack_.pop();
      if (n->left) pushRight(n->left);
      return *this;
    }

    bool operator!=(const rnl_iterator& other) const
    {
      return stack_ != other.stack_;
    }

    bool operator==(const rnl_iterator& other) const
    {
      return stack_ == other.stack_;
    }

  private:
    asafov::Stack< map< Key, Value >::node* > stack_;

    explicit rnl_iterator(map< Key, Value >::node* root)
    {
      pushRight(root);
    }

    void pushRight(map< Key, Value >::node* n)
    {
      while (n)
      {
        stack_.push(n);
        n = n->right;
      }
    }
  };
}

#endif
