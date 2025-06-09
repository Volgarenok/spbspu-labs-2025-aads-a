#ifndef TWOTHREEITERATORS_HPP
#define TWOTHREEITERATORS_HPP
#include <iterator>
#include "TwoThreeNode.hpp"

namespace zakirov
{
  template< typename T, typename K >
  class TwoThreeIter: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    ~TwoThreeIter() = default;
    T & operator*() const;
    T * operator->() const;
    TwoThreeIter< T, K > & operator++();
    TwoThreeIter< T, K > operator++(int);
    TwoThreeIter< T, K > & operator--();
    TwoThreeIter< T, K > operator--(int);
    bool operator!=(const TwoThreeIter< T, K > &) const;
    bool operator==(const TwoThreeIter< T, K > &) const;
  private:
    TwoThreeIter(TwoThreeNode< T, K > * node);
    TwoThreeNode< T, K > * node_;
    bool first_ptr_;
  };

  template< typename T, typename K >
  TwoThreeIter< T, K >::TwoThreeIter(TwoThreeNode< T, K > * node):
    node_(node),
    first_ptr_(true)
  {}

  template< typename T, typename K >
  T & TwoThreeIter< T, K >::operator*() const
  {
    return node_->data_;
  }

  template< typename T, typename K >
  T * TwoThreeIter< T, K >::operator->() const
  {
    return std::addressof(node_->data_);
  }

/*
  template< typename T, typename K >
  TwoThreeIter< T, K > & TwoThreeIter< T, K >::operator++()
  {
    if (first_ptr_)
    {
      if (node_->middle_)
      {
        node_ = node_->middle_;
        find_min_(node_);
      }
      else if (node_->second_exists_)
      {
        first_ptr_ = false;
      }
      else if ()
      {

      }
    }
    else
    {
      if (node_->right_)
      {
        node_ = node_->right_;
        find_min_(node_);
      }
    }
  }

  template< typename T, typename K >
  TwoThreeIter< T, K > TwoThreeIter< T, K >::operator++(int)
  {

  }

  template< typename T, typename K >
  TwoThreeIter< T, K > & TwoThreeIter< T, K >::operator--()
  {

  }

  template< typename T, typename K >
  TwoThreeIter< T, K > TwoThreeIter< T, K >::operator--(int)
  {

  }

  template< typename T, typename K >
  bool TwoThreeIter< T, K >::operator!=(const TwoThreeIter< T, K > &) const
  {

  }

  template< typename T, typename K >
  bool TwoThreeIter< T, K >::operator==(const TwoThreeIter &) const
  {

  }
*/
}

#endif
