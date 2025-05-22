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
      size_t size = capacity;
      capacity *= 2;
      T * temp = new T[capacity];
      try
      {
        for (size_t i = 0; i < size; i++)
        {
          temp[i] = data[i];
        }
      }
      catch (const std::exception &)
      {
        delete[] temp;
        throw;
      }
      delete[] data;
      return temp;
    }
  }
}

#endif
