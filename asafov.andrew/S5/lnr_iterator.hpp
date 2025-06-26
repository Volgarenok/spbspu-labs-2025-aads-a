#ifndef LNR_ITERATOR_HPP
#define LNR_ITERATOR_HPP
#include <map.hpp>
#include <stack.hpp>
#include <utility>

namespace asafov
{
  template< typename Key, typename Value >
  class lnr_iterator
  {
    friend class map< Key, Value >;

  public:
    lnr_iterator() = default;

    std::pair< Key, Value >& operator*() const
    {
      return stack_.top()->data;
    }

    std::pair< Key, Value >* operator->() const
    {
      return &(stack_.top()->data);
    }

    lnr_iterator& operator++()
    {
      map< Key, Value >::node* n = stack_.top();
      stack_.pop();
      if (n->right) pushLeft(n->right);
      return *this;
    }

    bool operator!=(const lnr_iterator& other) const
    {
      return stack_ != other.stack_;
    }

    bool operator==(const lnr_iterator& other) const
    {
      return stack_ == other.stack_;
    }

  private:
    asafov::Stack< map< Key, Value >::node* > stack_;

    explicit lnr_iterator(map< Key, Value >::node* root)
    {
      pushLeft(root);
    }

    void pushLeft(map< Key, Value >::node* n)
    {
      while (n)
      {
        stack_.push(n);
        n = n->left;
      }
    }
  };
}

#endif
