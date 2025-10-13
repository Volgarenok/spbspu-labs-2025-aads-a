#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <list>

namespace shabalin
{
  template < typename T >
  class Queue
  {
  public:
    bool empty() const
    {
      return queue_list.empty();
    }
    size_t size() const
    {
      return queue_list.size();
    }
    T & front()
    {
      return queue_list.front();
    }
    const T & front() const
    {
      return queue_list.front();
    }
    T & back()
    {
      return queue_list.back();
    }
    const T & back() const
    {
      return queue_list.back();
    }
    void push(const T & rhs)
    {
      queue_list.push_back(rhs);
    }
    void pop()
    {
      queue_list.pop_front();
    }
  private:
    std::list< T > queue_list;
  };
}

#endif
