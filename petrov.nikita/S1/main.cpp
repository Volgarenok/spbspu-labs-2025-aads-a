#include "forward_ring_list.hpp"
#include <string>
#include <iostream>
#include <limits>

int main()
{
  petrov::ForwardRingList< std::pair< std::string, petrov::ForwardRingList< size_t > > > head = {};
  std::string sequence_num = {};
  size_t number = 0;
  while (!std::cin.eof())
  {
    std::cin.clear();
    std::cin >> sequence_num;
    petrov::ForwardRingList< size_t > subhead = {};
    while (!std::cin.eof() && std::cin)
    {
      std::cin >> number;
      if (!std::cin)
      {
        break;
      }
      subhead.push_front(number);
    }
    subhead.reverse();
    head.push_front({ sequence_num, subhead });
  }
  head.reverse();
  if (sequence_num.empty())
  {
    std::cout << 0;
    std::cout << "\n";
    return 0;
  }
  auto it = head.cbegin();
  if (head.size() == 1)
  {
    std::cout << it->first;
  }
  else
  {
    std::cout << (it++)->first;
    do
    {
      std::cout << " " << it->first;
    }
    while (it++ != head.cend());
  }
  std::cout << "\n";
  petrov::ForwardRingList< size_t > sums = {};
  do
  {
    size_t sum = 0;
    auto prev_it_out = head.end();
    auto it_out = head.begin();
    do
    {
      if (it_out->second.empty() && head.size() != 1)
      {
        auto val = *it_out;
        head.remove(val);
        it_out = prev_it_out;
      }
      else if (it_out->second.empty() && head.size() == 1)
      {
        head.pop_front();
        break;
      }
      else
      {
        auto it = it_out->second.begin();
        if (it_out == head.begin())
        {
          std::cout << *it;
          if (sum <= std::numeric_limits< size_t >::max() - *it)
          {
            sum += *it;
            it_out->second.pop_front();
          }
          else
          {
            std::cout << "\n";
            std::cerr << "ERROR: Overflow" << "\n";
            return 1;
          }
        }
        else
        {
          std::cout << " " << *it;
          if (sum <= std::numeric_limits< size_t >::max() - *it)
          {
            sum += *it;
            it_out->second.pop_front();
          }
          else
          {
            std::cout << "\n";
            std::cerr << "ERROR: Overflow" << "\n";
            return 1;
          }
        }
        ++prev_it_out;
      }
    }
    while (it_out++ != head.end() && head.begin() != head.end());
    if (sum)
    {
      sums.push_front(sum);
      std::cout << "\n";
    }
  }
  while (!head.empty());
  if (sums.empty())
  {
    std::cout << 0;
  }
  else
  {
    sums.reverse();
    if (sums.size() == 1)
    {
      std::cout << *sums.cbegin();
    }
    else
    {
      auto it = sums.cbegin();
      std::cout << *(it++);
      do
      {
        std::cout << " " << *it;
      }
      while (it++ != sums.cend());
    }
  }
  std::cout << "\n";
}
