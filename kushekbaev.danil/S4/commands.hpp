#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iostream>
#include <map>
#include <limits>

template<typename Key, typename T>
using dict_t = std::map< Key, T >;
template<typename Key, typename T>
using dataset_t = std::map<std::string, dict_t< Key, T >>;

namespace kushekbaev
{
  template<typename Key, typename T>
  void executeCommand(std::istream& in, std::ostream& out, dataset_t< Key, T >& dataset);

  template<typename Key, typename T>
  void print(std::ostream& out, const std::string& name, const dataset_t< Key, T >& dataset);

  template<typename Key, typename T>
  void complement(dict_t< Key, T >& result, const dict_t< Key, T >& ds1, const dict_t< Key, T >& ds2);

  template<typename Key, typename T>
  void intersect(dict_t< Key, T >& result, const dict_t< Key, T >& ds1, const dict_t< Key, T >& ds2);

  template<typename Key, typename T>
  void unification(dict_t< Key, T >& result, const dict_t< Key, T >& ds1, const dict_t< Key, T >& ds2);

  template<typename Key, typename T>
  void executeCommand(std::istream& in, std::ostream& out, dataset_t< Key, T >& dataset)
  {
    std::string command;
    in >> command;

    if (command == "print")
    {
      std::string name;
      in >> name;
      print(out, name, dataset);
    }
    else if (command == "complement" || command == "intersect" || command == "union")
    {
      std::string resultName, name1, name2;
      in >> resultName >> name1 >> name2;

      if (dataset.find(name1) == dataset.end() || dataset.find(name2) == dataset.end())
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
      dict_t< Key, T > resultDict;
      const auto& dict1 = dataset.at(name1);
      const auto& dict2 = dataset.at(name2);

      if (command == "complement")
      {
        complement(resultDict, dict1, dict2);
      }
      else if (command == "intersect")
      {
        intersect(resultDict, dict1, dict2);
      }
      else if (command == "union")
      {
        unification(resultDict, dict1, dict2);
      }

      dataset[resultName] = resultDict;
    }
    else if (in.eof())
    {
      return;
    }
    else
    {
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      throw std::logic_error("<INVALID COMMAND>");
    }
  }

  template<typename Key, typename T>
  void print(std::ostream& out, const std::string& name, const dataset_t< Key, T >& dataset)
  {
    if (dataset.empty())
    {
      throw std::logic_error("<EMPTY>");
    }

    auto it = dataset.find(name);
    if (it == dataset.end())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }

    out << name;
    for (typename dict_t< Key, T >::const_iterator dict_it = it->second.begin(); dict_it != it->second.end(); ++dict_it)
    {
      out << " " << dict_it->first << " " << dict_it->second;
    }
    out << "\n";
  }

  template<typename Key, typename T>
  void complement(dict_t< Key, T >& result, const dict_t< Key, T >& ds1, const dict_t< Key, T >& ds2)
  {
    auto it1 = ds1.begin();
    auto it2 = ds2.begin();

    while (it1 != ds1.end() && it2 != ds2.end())
    {
      if (it1->first < it2->first)
      {
        result.insert(*it1++);
      }
      else if (it2->first < it1->first)
      {
        result.insert(*it2++);
      }
      else
      {
        ++it1;
        ++it2;
      }
    }
    result.insert(it1, ds1.end());
    result.insert(it2, ds2.end());
  }

  template<typename Key, typename T>
  void intersect(dict_t< Key, T >& result, const dict_t< Key, T >& ds1, const dict_t< Key, T >& ds2)
  {
    auto it1 = ds1.begin();
    auto it2 = ds2.begin();

    while (it1 != ds1.end() && it2 != ds2.end())
    {
      if (it1->first < it2->first) ++it1;
      else if (it2->first < it1->first) ++it2;
      else
      {
        result.insert(*it1);
        ++it1;
        ++it2;
      }
    }
  }

  template<typename Key, typename T>
  void unification(dict_t< Key, T >& result, const dict_t< Key, T >& ds1, const dict_t< Key, T >& ds2)
  {
    result = ds1;
    for (typename dict_t< Key, T >::const_iterator it = ds2.begin(); it != ds2.end(); ++it)
    {
      result[it->first] = it->second;
    }
  }
}

#endif
