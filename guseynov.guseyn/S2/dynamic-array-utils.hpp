#ifndef DYNAMIC_ARRAY_UTILS_HPP
#define DYNAMIC_ARRAY_UTILS_HPP

#include <cstddef>
#include <stdexcept>

namespace guseynov
{
  template< typename T >
  T * resizeArray(T * data, size_t & capacity)
  {
    {
      size_t newCapacity = capacity * 2;
      T * newData = new T[newCapacity];
      try
      {
        for (size_t i = 0; i < capacity; i++)
        {
          newData[i] = data[i];
        }
      }
      catch (...)
      {
        delete[] newData;
        throw;
      }
      capacity = newCapacity;
      delete[] data;
      return newData;
    }
  }
}

#endif
