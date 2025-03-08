#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "CircularFwdList.hpp"

int main()
{
  using list_elem_type = unsigned long;
  using IntList = zholobov::CircularFwdList< list_elem_type >;
  using ListElem = std::pair< std::string, IntList >;
  zholobov::CircularFwdList< ListElem > list;
  zholobov::CircularFwdList< list_elem_type > sum_list;
  bool is_overflow = false;
  try {
    while (std::cin.good()) {
      std::string line;
      std::getline(std::cin, line);
      std::stringstream ss(line);
      std::string name;
      IntList int_list;
      if (!(ss >> name)) {
        break;
      }
      list_elem_type elem = 0;
      while (ss >> elem) {
        int_list.push_back(elem);
      }
      ListElem list_elem(std::move(name), std::move(int_list));
      list.push_back(std::move(list_elem));
    }

    zholobov::CircularFwdList< std::pair< IntList::const_iterator, IntList::const_iterator > > iter_list;
    if (!list.empty()) {
      bool first = true;
      for (auto it = list.begin(); it != list.end(); ++it) {
        if (first) {
          std::cout << it->first;
          first = false;
        } else {
          std::cout << " " << it->first;
        }
        iter_list.push_back(std::make_pair(it->second.cbegin(), it->second.cend()));
      }
      std::cout << "\n";
    }

    bool is_done_printing = true;
    do {
      is_done_printing = true;
      list_elem_type sum = 0;
      bool first = true;
      for (auto it = iter_list.begin(); it != iter_list.end(); ++it) {
        if (it->first != it->second) {
          list_elem_type val = *(it->first);
          if (first) {
            std::cout << val;
            first = false;
          } else {
            std::cout << " " << val;
          }
          ++it->first;
          is_done_printing = false;
          if (sum <= std::numeric_limits< list_elem_type >::max() - val) {
            sum += val;
          } else {
            is_overflow = true;
          }
        }
      }
      if (!is_done_printing) {
        std::cout << "\n";
        sum_list.push_back(sum);
      }
    } while (!is_done_printing);
  } catch (const std::bad_alloc& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  if (is_overflow) {
    std::cerr << "Overflow error\n";
    return 1;
  }

  if (sum_list.empty()) {
    std::cout << "0\n";
  } else {
    auto it = sum_list.begin();
    std::cout << *it++;
    for (; it != sum_list.end(); ++it) {
      std::cout << " " << *it;
    }
    std::cout << "\n";
  }

  return 0;
}
