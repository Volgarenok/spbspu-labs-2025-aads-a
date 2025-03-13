#ifndef LIST_HPP
#define LIST_HPP
#include <list>
#include <cstddef>

namespace smirnov
{
  template< typename T >
  class List
  {
  public:
    List() = default;
    void pushBack(const T & value);
    bool isEmpty() const;
    size_t getSize() const;
    void popFront();
    using ConstIterator = typename std::list< T >::const_iterator;
    ConstIterator begin() const;
    ConstIterator end() const;
  private:
    std::list< T > data;
  };
}

template< typename T >
void smirnov::List< T >::pushBack(const T & value)
{
  data.push_back(value);
}

template< typename T >
bool smirnov::List< T >::isEmpty() const
{
  return data.empty();
}

template< typename T >
size_t smirnov::List< T >::getSize() const
{
  return data.size();
}

template< typename T >
void smirnov::List< T >::popFront()
{
  if (!data.empty())
  {
    data.pop_front();
  }
}

template< typename T >
typename smirnov::List< T >::ConstIterator smirnov::List< T >::begin() const
{
  return data.begin();
}

template< typename T >
typename smirnov::List< T >::ConstIterator smirnov::List< T >::end() const
{
  return data.end();
}
#endif
