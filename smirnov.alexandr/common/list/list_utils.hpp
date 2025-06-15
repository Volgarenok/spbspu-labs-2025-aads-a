#ifndef LIST_UTILS_HPP
#define LIST_UTILS_HPP
#include "list.hpp"

namespace smirnov
{
  template < typename T >
  void pushBack(List< T > & list, const T & value)
  {
    if (list.empty())
    {
      list.push_front(value);
      return;
    }

    List< T > temp;
    while (!list.empty())
    {
      temp.push_front(list.front());
      list.pop_front();
    }
    temp.push_front(value);
    while (!temp.empty())
    {
      list.push_front(temp.front());
      temp.pop_front();
    }
  }
}
#endif
