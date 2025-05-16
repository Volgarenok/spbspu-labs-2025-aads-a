#include "outSequance.hpp"
#include <cstddef>
#include <iostream>
#include <limits>

using Seq = dribas::List< std::pair< std::string, dribas::List< unsigned long long > > >;

void dribas::outSequanceName(std::ostream& out, const Seq& sequance)
{
  out << sequance.begin()->first;
  for (auto i = ++sequance.begin(); i != sequance.end(); i++) {
    out << ' ' << i->first;
  }
}

void dribas::outSequanceNameSum(std::ostream& out, const Seq& sequance, bool& isOverflow) {
  size_t maxSize = 0;
  for (auto i = sequance.begin(); i != sequance.end(); ++i) {
    maxSize = std::max(maxSize, i->second.size());
  }

  if (maxSize == 0) {
    out << '0';
    return;
  }

  dribas::List< unsigned long long > sums;
  for (size_t i = 0; i < maxSize; ++i) {
    auto first_elem = sequance.begin();
    while (first_elem != sequance.end() && i >= first_elem->second.size()) {
      ++first_elem;
    }
    if (first_elem != sequance.end()) {
      auto it_data = first_elem->second.begin();
      std::advance(it_data, i);
      out << *it_data;
      unsigned long long sum = *it_data;
      if (*it_data > std::numeric_limits< int >::max()) {
        isOverflow = true;
      }
      for (auto j = ++first_elem; j != sequance.end(); ++j) {
        if (i < j->second.size()) {
          it_data = j->second.begin();
          std::advance(it_data, i);
          out << " " << *it_data;
          if (*it_data > std::numeric_limits< int >::max() - sum) {
            isOverflow = true;
          }
          sum += *it_data;
        }
      }
      sums.push_back(sum);
    }

    if (!(i == maxSize - 1 && isOverflow)) {
      out << '\n';
    }
  }
  if (!isOverflow && !sums.empty()) {
    out << sums.front();
    for (auto it = ++sums.begin(); it != sums.end(); ++it) {
      out << " " << *it;
    }
  }
}
