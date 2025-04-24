#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <stdexcept>
#include <sstream>
#include <utility>
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
      ~List();
      List(const List< T > &);
      List(List< T > &&);
      List< T > & operator=(const List< T > &);
      List< T > & operator=(List< T > &&);
      void clear();
      ListNode< T > * head() const;
      ListNode< T > * tail() const;
      void push_back(const T &);
      void push_back(T &&);
      void push_front(const T &);
      void push_front(T &&);
      size_t size();
      bool empty() const noexcept;
      T & front();
      T & back();
      void pop_back();
      void pop_front();

      ListIterator< T > begin();
      ListIterator< T > end();
      ConstListIterator< T > cbegin() const;
      ConstListIterator< T > cend() const;
      std::stringstream & out(std::stringstream &out);

    private:
      ListNode< T > *fake_;
      size_t size_;
      void release();
  };

  template< class T >
  void List< T >::release()
  {
    fake_ = nullptr;
    size_ = 0;
  }

  template< class T >
  void List< T >::clear()
  {
    while (!empty())
    {
      pop_back();
    }
    delete fake_;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T >::List(const List< T > &rhs):
    fake_(T{}),
    size_(0)
  {
    for (auto cur = rhs.cbegin(); cur != rhs.cend(); ++cur)
    {
      push_back(*cur);
      size_++;
    }
  }

  template< class T >
  List< T >::List(List< T > &&rhs):
    fake_(rhs.fake_),
    size_(rhs.size_)
  {
    rhs.release();
  }

  template< class T >
  List< T > & List<T>::operator=(const List< T > &rhs)
  {
    clear();
    size_ = 0;
    fake_ = ListNode< T >(T{});
    for (auto cur = rhs.cbegin(); cur != rhs.cend(); ++cur)
    {
      push_back(*cur);
      size_++;
    }
    return *this;
  }

  template< class T >
  List< T > & List<T>::operator=(List< T > &&rhs)
  {
    clear();
    fake_ = rhs.fake_;
    size_ = rhs.size();
    rhs.release();
    return *this;
  }

  template< class T >
  size_t List< T >::size()
  {
    return size_;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return (size_ == 0);
  }

  template< class T >
  ListNode< T > * List< T >::head() const
  {
    if (empty())
    {
      throw std::out_of_range("list is empty");
    }
    return fake_->next_;
  }

  template< class T >
  ListNode< T > * List< T >::tail() const
  {
    if (empty())
    {
      throw std::out_of_range("list is empty");
    }
    return fake_->prev_;
  }

  template< class T >
  T & List< T >::front()
  {
    if (empty())
    {
      throw std::out_of_range("list is empty");
    }
    return head()->data_;
  }

  template< class T >
  T & List< T >::back()
  {
    if (empty())
    {
      throw std::out_of_range("list is empty");
    }
    return tail()->data_;
  }

  template< class T >
  List< T >::List():
    fake_(T{}),
    size_(0)
  {
    fake_->next_ = fake_;
    fake_->prev_ = fake_;
  }

  template< class T >
  void List< T >::push_front(const T &data)
  {
    ListNode< T > *cur = new ListNode< T >{ data };
    fake_->next_->prev_ = cur;
    cur->prev_ = fake_;
    cur->next_ = fake_->next_;
    fake_->next_ = cur;
    size_++;
  }

  template< class T >
  void List< T >::push_front(T &&data)
  {
    ListNode< T > *cur = new ListNode< T >{ data };
    fake_->next_->prev_ = cur;
    cur->prev_ = fake_;
    cur->next_ = fake_->next_;
    fake_->next_ = cur;
    size_++;
  }

  template< class T >
  void List< T >::push_back(const T &data)
  {
    ListNode< T > *cur = new ListNode< T >{ data };
    fake_->prev_->next_ = cur;
    cur->prev_ = fake_->prev_;
    fake_->prev_ = cur;
    cur->next_ = fake_;
    size_++;
  }

  template< class T >
  void List< T >::push_back(T &&data)
  {
    ListNode< T > *cur = new ListNode< T >{ data };
    fake_->prev_->next_ = cur;
    cur->prev_ = fake_->prev_;
    fake_->prev_ = cur;
    cur->next_ = fake_;
    size_++;
  }

  template< class T >
  void List< T >::pop_back()
  {
    if (empty())
    {
      throw std::logic_error("list is empty");
    }
    ListNode< T > *cur = tail();
    cur->prev_->next_ = fake_;
    fake_->prev_ = cur->prev_;
    size_--;
    delete cur;
  }

  template< class T >
  void List< T >::pop_front()
  {
    if (empty())
    {
      throw std::logic_error("list is empty");
    }
    ListNode< T > *cur = head();
    cur->next_->prev_ = fake_;
    fake_->next_ = cur->next_;
    size_--;
    delete cur;
  }

  template< class T >
  std::stringstream & List< T >::out(std::stringstream &out)
  {
    if (empty())
    {
      throw std::logic_error("list is empty");
    }
    ListNode< T > *cur = fake_->next_;
    out << cur->data_;
    cur = cur->next_;
    while (cur != fake_)
    {
      out << ' ' << cur->data_;
      cur = cur->next_;
    }
    return out;
  }

  template< class T >
  ListIterator< T > List< T >::begin()
  {
    return ListIterator< T >{ head() };
  }

  template< class T >
  ListIterator< T > List< T >::end()
  {
    return ListIterator< T >{ fake_ };
  }


  template< class T >
  ConstListIterator< T > List< T >::cbegin() const
  {
    return ConstListIterator< T >{ head() };
  }

  template< class T >
  ConstListIterator< T > List< T >::cend() const
  {
    return ConstListIterator< T >{ fake_ };
  }
}
#endif
