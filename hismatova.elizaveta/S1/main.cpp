#include <iostream>
#include <string>
#include <numeric>
#include <limits>
#include "list.hpp"

int main()
{
  using namespace hismatova;
  List< std::pair< std::string, List< unsigned long long > > > sequences;
  std::string name;
  while (std::cin >> name)
  {
    List< unsigned long long > numbers;
    unsigned long long num;
    while (std::cin >> num)
    {
      numbers.push_back(num);
    }
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
    }
    sequences.push_back({name, numbers});
  }
  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  const char* sep = "";
  for (const auto& pair: sequences)
  {
    const auto& first = pair.first;
    std::cout << sep << first;
    sep = " ";
  }
  std::cout << "\n";
  size_t max_length = 0;
  for (const auto& seq: sequences)
  {
    max_length = std::max(max_length, seq.second.size());
  }
  List< List< unsigned long long > > vertical_sequences;
  for (size_t i = 0; i < max_length; ++i)
  {
    List< unsigned long long > inner_list;
    for (const auto& seq: sequences)
    {
      if (i < seq.second.size())
      {
        auto it = seq.second.begin();
        for (size_t j = 0; j < i; ++j)
        {
          ++it;
        }
        inner_list.push_back(*it);
      }
    }
    vertical_sequences.push_back(inner_list);
  }
  for (const auto& v_seq: vertical_sequences)
  {
    const char* sep = "";
    for (const auto& item: v_seq)
    {
      std::cout << sep << item;
      sep = " ";
    }
    std::cout << "\n";
  }
  List< unsigned long long > sums;
  bool overflow_occurred = false;
  for (const auto& v_seq: vertical_sequences)
  {
    unsigned long long sum = 0;
    for (const auto& item: v_seq)
    {
      unsigned long long prev_sum = sum;
      sum += item;
      if (sum < prev_sum)
      {
        overflow_occurred = true;
        break;
      }
    }
    sums.push_back(sum);
  }
  if (overflow_occurred)
  {
    std::cerr << "ERROR: overflow\n";
    return 1;
  }
  if (sums.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  sep = "";
  for (const auto& item: sums)
  {
    std::cout << sep << item;
    sep = " ";
  }
  std::cout << "\n";
  return 0;
}
