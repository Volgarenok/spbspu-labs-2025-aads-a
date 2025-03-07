#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <stdexcept>
#include <sstream>
#include "const_list_iterator.hpp"
#include "list_node.hpp"
#include "list_iterator.hpp"
namespace evstyunichev
{
  template< class T >
  class List
  {
    public:
      List();
      void clear();
      void push_back(T data);
      size_t size();
      bool empty();
      T & front();
      T & back();
      void pop_back();
      void pop_front();

      ListIterator< T > begin();
      ListIterator< T > end();
      ConstListIterator< T > cbegin();
      ConstListIterator< T > cend();
      std::stringstream & out(std::stringstream &out);

  
    private:
      ListNode< T > *temp_;
      ListNode< T > *head_;
      ListNode< T > *tail_;
      size_t size_;
  };

  template< class T >
  void List< T >::clear()
  {
    for (auto i = begin(); i != end(); i++)
    {
      delete *i;
    }
  }

  template< class T >
  size_t List< T >::size()
  {
    return size_;
  }

  template< class T >
  bool List< T >::empty()
  {
    return (size_ == 0);
  }

  template< class T >
  T & List< T >::front()
  {
    if (empty())
    {
      throw std::out_of_range("list is empty");
    }
    return head_->data_;
  }

  template< class T >
  T & List< T >::back()
  {
    if (empty())
    {
      throw std::out_of_range("list is empty");
    }
    return tail_->data_;
  }

  template< class T >
  List< T >::List():
    temp_(new ListNode< T >{T{}}),
    head_(temp_),
    tail_(temp_),
    size_(0)
  {
  }
  template< class T >
  void List< T >::push_back(T data)
  {
    tail_->next_ = new ListNode< T >{data};
    tail_->next_->prev_ = tail_;
    tail_ = tail_->next_;
    tail_->next_ = temp_;
    if (empty())
    {
      head_ = tail_;
    }
    size_++;
  }

  template< class T >
  void List< T >::pop_back()
  {
    if (empty())
    {
      throw std::logic_error("list is empty");
    }
    ListNode< T > *cur = tail_->prev_;
    cur->next_ = temp_;
    tail_ = cur;
    size_--;
  }

  template< class T >
  void List< T >::pop_front()
  {
    if (empty())
    {
      throw std::logic_error("list is empty");
    }
    head_ = head_->next_;
    size_--;
  }

  template< class T >
  std::stringstream & List< T >::out(std::stringstream &out)
  {
    if (empty())
    {
      throw std::logic_error("list is empty");
    }
    ListNode< T > *cur = head_;
    out << cur->data_;
    cur = cur->next_;
    while (cur != temp_)
    {
      out << ' ' << cur->data_;
      cur = cur->next_;
    }
    return out;
  }

  template< class T >
  ListIterator< T > List< T >::begin()
  {
    return ListIterator< T >{head_};
  }

  template< class T >
  ListIterator< T > List< T >::end()
  {
    return ListIterator< T >{temp_};
  }


  template< class T >
  ConstListIterator< T > List< T >::cbegin()
  {
    return ConstListIterator< T >{head_};
  }

  template< class T >
  ConstListIterator< T > List< T >::cend()
  {
    return ConstListIterator< T >{temp_};
  }
}
#endif
