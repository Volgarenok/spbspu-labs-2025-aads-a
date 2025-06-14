#ifndef NODELIST_HPP
#define NODELIST_HPP
namespace karnauhova
{
  template< typename T >
  struct NodeList
  {
    T data;
    NodeList< T >* next;
  };
}
#endif
