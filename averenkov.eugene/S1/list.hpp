#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "iteratorc.hpp"
#include "iterator.hpp"

namespace averenkov
{
  template< class T >
  class List
  {

  public:
    List();
    List(const List< T >&);
    List(List< T >&&);
    List(size_t, const T&);
    ~List();

    using Iter_t = ListIterator< T >;
    using Iterc_t = ConstListIterator< T >;

    Iter_t begin() const noexcept;
    Iter_t end() const noexcept;

    Iterc_t cbegin() const noexcept;
    Iterc_t cend() const noexcept;

    T& front() const noexcept;
    T& back() const noexcept;

    bool empty() const noexcept;

    size_t size() const noexcept;

    void push_front(const T&) ;
    void push_back(const T&);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(List&) noexcept;
    void clear() noexcept;

    void fill(Iter_t first, Iter_t last, const T&);

    void remove(const T&);
    template< class UnaryPredicate >
    void removeIf(UnaryPredicate);

    void splice(Iterc_t pos, List< T >&);
    void splice(Iterc_t pos, List< T >&&);
    void splice(Iterc_t pos, List< T >&, Iterc_t it);
    void splice(Iterc_t pos, List< T >&&, Iterc_t it);
    void splice(Iterc_t pos, List< T >&, Iterc_t first, Iterc_t last);
    void splice(Iterc_t pos, List< T >&&, Iterc_t first, Iterc_t last);

    void assign(size_t, const T&);

  private:

    Node< T >* fake_;
    Node< T >* tail_;
    size_t size_;

  };

  template < class T >
  List< T >::List():
    size_(0)
  {
    fake_ = new Node< T >(T());
    fake_->next_ = fake_;
  }

  template < class T >
  List< T >::List(const List< T >& rhs):
    List()
  {
    Node< T >* current = rhs.fake_->next_;
    while (current != rhs.fake_)
    {
      push_back(current->data_);
      current = current->next_;
    }
  }

  template < class T >
  List< T >::List(List< T >&& rhs):
    List()
  {
    swap(rhs);
  }

  template < class T >
  List< T >::List(size_t count, const T& value):
    List()
  {
    while (count--)
    {
      push_back(value);
    }
  }

  template < class T >
  List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template < class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template < class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class T >
  typename List< T >::Iter_t List< T >::begin() const noexcept
  {
    return Iter_t(fake_->next_);
  }

  template < class T >
  typename List< T >::Iter_t List< T >::end() const noexcept
  {
    return Iter_t(fake_);
  }

  template < class T >
  typename List< T >::Iterc_t List<T>::cbegin() const noexcept
  {
    return Iterc_t(fake_->next_);
  }

  template < class T>
  typename List< T >::Iterc_t List<T>::cend() const noexcept
  {
    return Iterc_t(fake_);
  }

  template < class T >
  T& List< T >::front() const noexcept
  {
    assert(!empty());
    return fake_->next_->data_;
  }

  template < class T >
  T& List< T >::back() const noexcept
  {
    assert(!empty());
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      current = current->next_;
    }
    return current->data_;
  }

  template < class T >
  void List< T >::push_front(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    newNode->next_ = fake_->next_;
    fake_->next_ = newNode;
    size_++;
  }

  template < class T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    newNode->next_ = fake_;
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      current = current->next_;
    }
    current->next_ = newNode;
    size_++;
  }

  template < class T >
  void List< T >::pop_front() noexcept
  {
    assert(!empty());
    Node< T >* temp = fake_->next_;
    fake_->next_ = temp->next_;
    delete temp;
    size_--;
  }

  template < class T >
  void List< T >::pop_back() noexcept
  {
    assert(!empty());
    Node< T >* current = fake_;
    while (current->next_->next_ != fake_)
    {
      current = current->next_;
    }
    delete current->next_;
    current->next_ = fake_;
    size_--;
  }

  template < class T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template < class T >
  void List< T >::swap(List& rhs) noexcept
  {
    std::swap(fake_, rhs.fake_);
    std::swap(size_, rhs.size_);
  }

  template < class T >
  void List< T >::remove(const T& value)
  {
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      if (current->next_->data_ == value)
      {
        Node< T >* toDelete = current->next_;
        current->next_ = current->next_->next_;
        delete toDelete;
        size_--;
      }
      else
      {
        current = current->next_;
      }
    }
  }

  template < class T >
  template < class UnaryPredicate >
  void List< T >::removeIf(UnaryPredicate pred)
  {
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      if (pred(current->next_->data_))
      {
        Node< T >* toDelete = current->next_;
        current->next_ = current->next_->next_;
        delete toDelete;
        size_--;
      }
      else
      {
        current = current->next_;
      }
    }
  }

  template < class T >
  void List< T >::splice(Iterc_t pos, List< T >& rhs, Iterc_t first, Iterc_t last)
  {
    if (first == last || rhs.empty())
    {
      return;
    }
    Node< T >* posNode = pos.getNode();
    Node< T >* firstNode = first.getNode();
    Node< T >* lastNode = firstNode;
    Node< T >* prevNode = rhs.fake_;
    while (prevNode->next_ != firstNode)
    {
      prevNode = prevNode->next_;
    }
    while (lastNode->next_ != last.getNode())
    {
      lastNode = lastNode->next_;
    }
    prevNode->next_ = lastNode->next_;
    lastNode->next_ = posNode->next_;
    posNode->next_ = firstNode;
    size_t count = 0;
    Node< T >* temp = firstNode;
    while (temp != last.getNode())
    {
      count++;
      temp = temp->next_;
    }
    size_ += count;
    rhs.size_ -= count;
  }

  template < class T >
  void List< T >::splice(Iterc_t pos, List< T >& rhs)
  {
    splice(pos, rhs, rhs.cbegin(), rhs.cend());
  }

  template < class T >
  void List< T >::splice(Iterc_t pos, List< T >&& rhs)
  {
    splice(pos, rhs);
  }

  template < class T >
  void List< T >::splice(Iterc_t pos, List< T >& rhs, Iterc_t it)
  {
    Iterc_t next_ = it;
    next_++;
    splice(pos, rhs, it, next_);
  }

  template < class T >
  void List< T >::splice(Iterc_t pos, List< T >&& rhs, Iterc_t it)
  {
    splice(pos, rhs, it);
  }

  template < class T >
  void List< T >::splice(Iterc_t pos, List< T >&& rhs, Iterc_t first, Iterc_t last)
  {
    splice(pos, rhs, first, last);
  }

  template < class T >
  void List< T >::assign(size_t count, const T& value)
  {
    List< T > copy = this;
    try
    {
      while (count--)
      {
        push_back(value);
      }
    }
    catch (...)
    {
      copy.clear();
      return;
    }
    swap(copy);
  }
};


#endif
