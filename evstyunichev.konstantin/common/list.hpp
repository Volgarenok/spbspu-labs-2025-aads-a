#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <stdexcept>
#include <sstream>
#include <utility>
#include "const_list_iterator.hpp"
#include "list_iterator.hpp"
#include "list_node.hpp"
namespace evstyunichev
{
  template< class T >
  using cIter = ConstListIterator< T >;
  template< class T >
  using Iter = ListIterator< T >;
  template< class T >
  using Node = ListNode< T >;
  template< class T >
  class List
  {
    public:
      List();
      ~List();
      List(const List< T > &lhs);
      List(List< T > &&rhs);
      List(size_t size, const T &value);
      List(size_t size, T &&value);

      ListIterator< T > begin();
      ListIterator< T > end();
      cIter< T > cbegin() const;
      cIter< T > cend() const;

      T & front();
      T & back();

      const T & front() const;
      const T & back() const;

      void clear();
      void swap(List< T > &list2);
      void push_back(const T &value);
      void push_back(T &&value);
      void push_front(const T &value);
      void push_front(T &&value);
      void pop_back();
      void pop_front();

      size_t size() const noexcept;
      bool empty() const noexcept;

      void splice(Iter< T > pos, List< T > &l_list);
      void splice(Iter< T > pos, List< T > &&r_list);
      void splice(Iter< T > pos, List< T > &l_list, Iter< T > it);
      void splice(Iter< T > pos, List< T > &&r_list, Iter< T > it);
      void splice(Iter< T > pos, List< T > &l_list, Iter< T > first, Iter< T > last);
      void splice(Iter< T > pos, List< T > &&r_list, Iter< T > first, Iter< T > last);

      void remove(const T &value);
      template < class Predicate >
      void remove_if(Predicate pred) noexcept;
      void assign(size_t size, const T &value);

      std::stringstream & out(std::stringstream &out);

    private:
      ListNode< T > *fake_;
      size_t size_;
      ListNode< T > * head() const;
      ListNode< T > * tail() const;
      void release();
      void my_splice(Iter< T > pos, Iter< T > first, Iter< T > last);
      Iter< T > erase(Iter< T > pos);
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
  }

  template< class T >
  List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template< class T >
  List< T >::List(const List< T > &lhs):
    fake_(new Node< T >{}),
    size_(0)
  {
    if (!lhs.empty())
    {
      for (auto cur = lhs.cbegin(); cur != lhs.cend(); ++cur)
      {
        push_back(*cur);
        size_++;
      }
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
  List< T >::List(size_t size, const T &value):
    fake_(new Node< T >{}),
    size_(0)
  {
    for (; size; size--)
    {
      push_back(value);
    }
  }

  template< class T >
  List< T >::List(size_t size, T &&value):
    fake_(new Node< T >{}),
    size_(0)
  {
    for (; size; size--)
    {
      push_back(value);
    }
  }

  template< class T >
  void List<T>::swap(List< T > &list2)
  {
    std::swap(fake_, list2.fake_);
    std::swap(size_, list2.size_);
  }

  template< class T >
  size_t List< T >::size() const noexcept
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
    return fake_->next_;
  }

  template< class T >
  ListNode< T > * List< T >::tail() const
  {
    return fake_->prev_;
  }

  template< class T >
  T & List< T >::front()
  {
    if (empty())
    {
      throw std::out_of_range("list is empty front");
    }
    return head()->data_;
  }

  template< class T >
  T & List< T >::back()
  {
    if (empty())
    {
      throw std::out_of_range("list is empty back");
    }
    return tail()->data_;
  }

  template< class T >
  const T & List< T >::front() const
  {
    if (empty())
    {
      throw std::out_of_range("list is empty front");
    }
    return head()->data_;
  }

  template< class T >
  const T & List< T >::back() const
  {
    if (empty())
    {
      throw std::out_of_range("list is empty back");
    }
    return tail()->data_;
  }

  template< class T >
  List< T >::List():
    fake_(new ListNode< T >{}),
    size_(0)
  {}

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
      throw std::logic_error("list is empty pop_back");
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
      throw std::logic_error("list is empty pop_front");
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
      throw std::logic_error("list is empty out");
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

  template< class T >
  void List< T >::splice(Iter< T > pos, List< T > &l_list)
  {
    if (l_list.empty())
    {
      return;
    }
    my_splice(pos, l_list.begin(), l_list.end());
    size_ += l_list.size();
    l_list.size_ = 0;
    return;
  }

  template< class T >
  void List< T >::splice(Iter< T > pos, List< T > &&r_list)
  {
    if (r_list.empty())
    {
      return;
    }
    my_splice(pos, r_list.begin(), r_list.end());
    size_ += r_list.size();
    r_list.size_ = 0;
    return;
  }

  template< class T >
  void List< T >::splice(Iter< T > pos, List< T > &l_list, Iter< T > it)
  {
    if (it == l_list.cend())
    {
      throw std::out_of_range("iter to fake!");
    }
    Node< T > *it_node = it.getNode();
    my_splice(pos, it, cIter< T >{ it_node->next_ });
    size_++;
    l_list.size_--;
    return;
  }

  template< class T >
  void List< T >::splice(Iter< T > pos, List< T > &&r_list, Iter< T > it)
  {
    if (it == r_list.cend())
    {
      throw std::out_of_range("iter to fake!");
    }
    Node< T > *it_node = it.getNode();
    my_splice(pos, it, cIter< T >{ it_node->next_ });
    size_++;
    r_list.size_--;
    return;
  }

  template< class T >
  void List< T >::splice(Iter< T > pos, List< T > &l_list, Iter< T > first, Iter< T > last)
  {
    long long dist = std::distance(first, last);
    if (dist < 0)
    {
      throw std::logic_error("not right order for iters");
    }
    if (dist > 0)
    {
      my_splice(pos, first, last);
      size_ += dist;
      l_list.size_ -= dist;
    }
    return;
  }

  template< class T >
  void List< T >::splice(Iter< T > pos, List< T > &&r_list, Iter< T > first, Iter< T > last)
  {
    long long dist = std::distance(first, last);
    if (dist < 0)
    {
      throw std::logic_error("not right order for iters");
    }
    if (dist > 0)
    {
      size_ += dist;
      my_splice(pos, first, last);
      r_list.size_ -= dist;
    }
    return;
  }

  template< class T >
  void List< T >::my_splice(Iter< T > pos, Iter< T > first, Iter< T > last)
  {
    Node< T > *first_node = first.getNode(), *last_node = last.getNode();
    Node< T > *pos_node = pos.getNode(), *prelast_node = last_node->prev_;
    last_node->prev_ = first_node->prev_;
    first_node->prev_ = pos_node->prev_;
    prelast_node->next_ = pos_node;
    pos_node->prev_->next_ = first_node;
    pos_node->prev_ = prelast_node;
    last_node->prev_->next_ = last_node;
    return;
  }

  template< class T >
  void List< T >::remove(const T &value)
  {
    for (Iter< T > it = begin(); it != end(); ++it)
    {
      if (*it == value)
      {
        Iter< T > prev{ it.getNode()->prev_ };
        prev.getNode().next_ = it.getNode()->next_;
        it.getNode()->next_->prev_ = prev.getNode();
        delete it.getNode();
        it = prev;
      }
    }
    return;
  }

  template< class T >
  template< class Predicate >
  void List< T >::remove_if(Predicate pred) noexcept
  {
    for (Iter< T > it = begin(); it != end(); ++it)
    {
      if (pred(*it))
      {
        Iter< T > prev{ it.getNode()->prev_ };
        prev.getNode().next_ = it.getNode()->next_;
        it.getNode()->next_->prev_ = prev.getNode();
        delete it.getNode();
        it = prev;
      }
    }
    return;
  }

  template< class T >
  void List< T >::assign(size_t size, const T &value)
  {
    clear();
    for (; size_ < size; size_++)
    {
      push_back(value);
    }
    return;
  }
}
#endif
