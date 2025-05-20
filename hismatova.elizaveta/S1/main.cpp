#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <limits>
#include "list.hpp"

int main()
{
  using namespace hismatova;
  std::vector< std::pair< std::string, List< unsigned long long > > > sequences;
  std::string name;
  while (std::cin >> name)
  {
    List< unsigned long long > numbers;
    std::string line;
    std::getline(std::cin, line);
    if (line.empty())
    {
      line = "";
    }
    std::istringstream iss(line);
    unsigned long long num = 0;
    while (iss >> num)
    {
      numbers.push_back(num);
    }
    sequences.emplace_back(name, numbers);
  }
  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    std::cout << sequences[i].first;
    if (i + 1 < sequences.size())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  size_t max_length = 0;
  for (const auto& seq : sequences)
  {
    max_length = std::max(max_length, seq.second.size());
  }
  std::vector< std::vector< unsigned long long > > vertical_sequences(max_length);
  for (size_t i = 0; i < max_length; ++i)
  {
    for (const auto& seq : sequences)
    {
      if (i < seq.second.size())
      {
        auto it = seq.second.begin();
        for (size_t j = 0; j < i; ++j) ++it;
        vertical_sequences[i].push_back(*it);
      }
    }
  }
  for (const auto& v_seq : vertical_sequences)
  {
    for (size_t j = 0; j < v_seq.size(); ++j)
    {
      std::cout << v_seq[j];
      if (j + 1 < v_seq.size())
      {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }
  std::vector< unsigned long long > sums;
  bool overflow_occurred = false;
  for (const auto& v_seq : vertical_sequences)
  {
    unsigned long long sum = 0;
    for (size_t i = 0; i < v_seq.size(); ++i)
    {
      unsigned long long prev_sum = sum;
      sum += v_seq[i];
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
  for (size_t i = 0; i < sums.size(); ++i)
  {
    std::cout << sums[i];
    if (i +1 < sums.size())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  return 0;
}

