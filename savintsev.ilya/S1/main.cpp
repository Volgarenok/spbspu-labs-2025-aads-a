#include <iostream>
#include <string>
#include "list.hpp"

int main()
{
  using pair_t = std::pair< std::string, savintsev::List< size_t > >;
  using storage_t = savintsev::List< pair_t >;
  storage_t storage;

  std::string buffer = "";
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
  }

  if (buffer.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  {
    storage_t::iterator it = storage.begin();
    std::cout << it->first;
    for (++it; it != storage.end(); ++it)
    {
      std::cout << " " << it->first;
    }
    std::cout << "\n";
  }

  savintsev::List< size_t > sums;
  bool was_overflow = false;
  for (size_t i = 0; i < max_size; ++i)
  {
    size_t sum = 0;
    for (storage_t::iterator it = storage.begin(); it != storage.end(); ++it)
    {
      auto temp_it = it->second.begin();
      if (it->second.size() > i)
      {
        std::advance(temp_it, i);
        std::cout << (sum == 0 ? "" : " ") << *(temp_it);
        if (!((sum + *(temp_it) >= sum) && (sum + *(temp_it) >= *(temp_it))))
        {
          was_overflow = true;
        }
        sum += *(temp_it);
      }
    }
    sums.push_back(sum);
    std::cout << "\n";
  }
  if (errno == ERANGE || was_overflow)
  {
    std::cerr << "ERROR: some numbers are too big, overflow\n";
    return 1;
  }
  if (sums.empty())
  {
    sums.push_back(0ull);
  }

  {
    savintsev::List< size_t >::const_iterator it;
    it = sums.cbegin();
    std::cout << *it;
    for (++it; it != sums.cend(); ++it)
    {
      std::cout << " " << *it;
    }
    std::cout << "\n";
  }
}
