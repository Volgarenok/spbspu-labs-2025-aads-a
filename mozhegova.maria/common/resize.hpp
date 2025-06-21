#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <cstddef>
#include <stdexcept>

namespace mozhegova
{
  template< typename T >
  T * resize(T * data, size_t & capacity)
  {
    {
      size_t size = capacity * 2;
      T * temp = new T[size];
      try
      {
        for (size_t i = 0; i < capacity; i++)
        {
          temp[i] = data[i];
        }
      }
      catch (...)
      {
        delete[] temp;
        throw;
      }
      capacity *= 2;
      delete[] data;
      return temp;
    }
  }
}

#endif
