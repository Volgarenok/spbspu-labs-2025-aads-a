#ifndef INPUTDICTIONARIES_HPP
#define INPUTDICTIONARIES_HPP
#include <istream>
#include <list>
#include <string>
#include <map>

namespace zakirov
{
  template < class K, class T >
  void get_dictionaries(std::istream & in, std::map< std::string, std::map< K, T > > & dictionaries);

  template < class K, class T >
  void get_dictionaries(std::istream & in, std::map< std::string, std::map< K, T > > & dictionaries)
  {
    std::string name;
    std::map< K, T > dict;
    K key;
    T value;
    while (in)
    {
      in >> name;
      while (in && in.peek() != '\n')
      {
        in >> key >> value;
        dict.insert({key, value});
      }

      dictionaries.insert({name, dict});
      dict.clear();
    }
  }
}

#endif
