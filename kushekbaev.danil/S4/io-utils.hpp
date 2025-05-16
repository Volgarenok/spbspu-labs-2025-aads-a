#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <fstream>
#include <string>
#include <stdexcept>
#include <map>

template< typename Key, typename T >
using dictionary_t = std::map< Key, T >;
template< typename Key, typename T >
using dataset_t = std::map< std::string, dictionary_t< Key, T > >;

namespace kushekbaev
{
  template< typename Key, typename T >
  dataset_t< Key, T > readInputFromFile(std::ifstream& in)
  {
    if (!in)
    {
      throw std::runtime_error("File couldn't be open!");
    }
    std::string name;
    dataset_t< Key, T > dataset;
    while (in >> name)
    {
      Key key;
      T value;
      dictionary_t< Key, T > dictionary;
      while (in && in.peek() != '\n')
      {
        in >> key >> value;
        dictionary.insert({ key, value });
      }
      dataset.insert({ name, dictionary });
    }
    return dataset;
  }
}

#endif
