#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <list>
#include <stdexcept>
#include "../S1/fwd_list.hpp"

namespace zakirov
{
  template < typename T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue & other);
    Queue(Queue && other) noexcept;
    ~Queue() = default;
    bool empty();
    size_t size();
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    void push (const T & value);
    void push (T && value);
    void pop();
  private:
    FwdList< T > data_;
    FwdIterator< T > back_;
    CFwdIterator< T > cback_;
    FwdIterator< T > find_last(FwdList< T > & other);
    CFwdIterator< T > cfind_last(const FwdList< T > & other);
  };

  template < typename T >
  Queue< T >::Queue():
    back_(data_.before_begin()),
    cback_(data_.cend())
  {}

  template < typename T >
  Queue< T >::Queue(const Queue & other):
    data_(other.data_),
    back_(find_last(data_)),
    cback_(cfind_last(data_))
  {}

  template < typename T >
  Queue< T >::Queue(Queue && other) noexcept:
    data_(std::move(other.data_)),
    back_(find_last(data_)),
    cback_(cfind_last(data_))
  {}

  template < typename T >
  bool Queue< T >::empty()
  {
    return data_.empty();
  }

  template < typename T >
  size_t Queue< T >::size()
  {
    return data_.size();
  }

  template < typename T >
  T & Queue< T >::front()
  {
    return *(data_.begin());
  }

  template < typename T >
  const T & Queue< T >::front() const
  {
    return *(data_.cbegin());
  }

  template < typename T >
  T & Queue< T >::back()
  {
    return *back_;
  }

  template < typename T >
  const T & Queue< T >::back() const
  {
    return *cback_;
  }

  template < typename T >
  void Queue< T >::push (const T & value)
  {
    data_.insert_after(back_, value);
    ++back_;
    ++cback_;
  }

  template < typename T >
  void Queue< T >::push (T && value)
  {
    data_.insert_after(back_, value);
    ++back_;
    ++cback_;
  }

  template < typename T >
  void Queue< T >::pop()
  {
    data_.pop_front();
  }

  template < typename T >
  FwdIterator< T > Queue< T >::find_last(FwdList< T > & data)
  {
    FwdIterator< T > last_el = data.before_begin();
    FwdIterator< T > finder = data.begin();
    while (finder != data.end())
    {
      ++finder;
      ++last_el;
    }

    return last_el;
  }

  template < typename T >
  CFwdIterator< T > Queue< T >::cfind_last(const FwdList< T > & data)
  {
    CFwdIterator< T > last_el = data.cend();
    CFwdIterator< T > finder = data.cbegin();
    while (finder != data.cend())
    {
      ++finder;
      ++last_el;
    }

    return last_el;
  }
}

#endif
