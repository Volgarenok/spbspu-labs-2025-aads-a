#ifndef LIST_ALGORITHMS_HPP
#define LIST_ALGORITHMS_HPP

namespace savintsev
{
  template< class It, class T >
  void fill(It first, It last, const T & value);
}

template< class It, class T >
void savintsev::fill(It first, It last, const T & value)
{
  while (first != last)
  {
    *first = value;
    ++first;
  }
}

#endif
