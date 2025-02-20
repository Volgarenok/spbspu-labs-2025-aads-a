#include <list>
#include <iostream>
#include <string>
#include "list.hpp"

int main()
{
  using pair_t = std::pair<std::string, std::list<size_t>>;
  using storage_t = savintsev::List<pair_t>;
  storage_t storage;
  std::string buffer = "";
  bool was_input = false;
  size_t max_size = 0;
  std::cin >> buffer;
  while (!std::cin.eof())
  {
    pair_t temp_pair;
    temp_pair.first = buffer;
    std::cin >> buffer;
    size_t temp_max_size = 0;
    while (isdigit(buffer[0]) && !std::cin.eof())
    {
      temp_pair.second.push_back(strtoull(buffer.c_str(), nullptr, 10));
      temp_max_size++;
      std::cin >> buffer;
    }
    max_size = std::max(max_size, temp_max_size);
    storage.push_back(temp_pair);
    was_input = true;
  }
  if (!was_input)
  {
    std::cout << "0\n";
    return 0;
  }
  for (storage_t::iterator it = storage.begin(); it != storage.end(); ++it)
  {
    std::cout << it->first;
    if (std::next(it) != storage.end())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  std::list<size_t> sums;
  bool was_overflow = false;
  for (size_t i = 0; i < max_size; ++i)
  {
    size_t printed = 0;
    size_t sum = 0;
    for (storage_t::iterator it = storage.begin(); it != storage.end(); ++it)
    {
      auto temp_it = it->second.begin();
      if (it->second.size() > i)
      {
        for (size_t next = 0; next < i; ++next)
        {
          temp_it++;
        }
        if (printed != 0)
        {
          std::cout << " ";
        }
        std::cout << *(temp_it);
        if (!((sum + *(temp_it) >= sum) && (sum + *(temp_it) >= *(temp_it))))
        {
          was_overflow = true;
        }
        sum += *(temp_it);
        printed++;
      }
    }
    sums.push_back(sum);
    std::cout << "\n";
  }
  if (errno == ERANGE || was_overflow)
  {
    std::cerr << "WARNING: some numbers are too big, overflow\n";
    return 1;
  }
  if (sums.empty())
  {
    sums.push_back(0);
  }
  for (std::list<size_t>::iterator it = sums.begin(); it != sums.end(); ++it)
  {
    std::cout << *it;
    if (std::next(it) != sums.end())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
}

/*
first 1 1 1
second 2 2 2 2
third
fourth 4 4
*/
