#include <iostream>
#include <list>
#include <string>
#include <limits>

int main()
{
  std::list< std::pair< std::string, std::list < unsigned long long int > > > lists;
  std::string s;
  std::cin >> s;
  size_t count = 0;
  size_t lists_count = 0;
  while (std::cin)
  {
    std::pair< std::string, std::list< unsigned long long int > > bidir;
    bidir.first = s;
    std::cin >> s;
    while(std::cin && std::isdigit(s[0]))
    {
      bidir.second.push_back(std::strtoull(s.c_str(), nullptr, 10));
      std::cin >> s;
    }
    lists.push_back(bidir);
    count = std::max(count, bidir.second.size());
    ++lists_count;
  }
  if (!lists_count)
  {
    std::cout << "0\n";
    return 0;
  }
  for (auto iter = lists.begin(); iter != --lists.end(); ++iter)
  {
    std::cout << iter->first << " ";
  }
  std::cout << (--lists.end())->first << "\n";
  for (size_t i = 0; i < count; ++i)
  {
    bool flag = false;
    for (auto iter = lists.begin(); iter != --(lists.end()); ++iter)
    {
      if (iter->second.size() > i)
      {
        auto iter2 = std::next(iter->second.begin(), i);
        if (flag)
        {
          std::cout << " ";
        }
        std::cout << *iter2;
        flag = true;
      }
    }
    if ((--lists.end())->second.size() <= i)
    {
      std::cout << "\n";
    }
    else
    {
      std::cout << " " << *(std::next((--(lists.end()))->second.begin(), i)) << "\n";
    }
  }
  unsigned long long int *sums = new unsigned long long int[count];
  constexpr unsigned long long int max = std::numeric_limits< unsigned long long int >::max();
  for (size_t i = 0; i < count; ++i)
  {
    unsigned long long int sum = 0;
    for (auto iter = lists.begin(); iter != lists.end(); ++iter)
    {
      if (iter->second.size() > i)
      {
        auto iter2 = std::next(iter->second.begin(), i);
        if (*iter2 > max - sum)
        {
          std::cerr << "overflow\n";
          delete[] sums;
          return 1;
        }
        sum += *iter2;
      }
    }
    sums[i] = sum;
  }
  bool empty = true;
  for (size_t i = 0; i < count; ++i)
  {
    if (sums[i] != 0)
    {
      empty = false;
    }
  }
  if (empty)
  {
    std::cout << "0\n";
    delete[] sums;
    return 0;
  }
  for (size_t i = 0; i < count - 1; ++i)
  {
    std::cout << sums[i] << " ";
  }
  std::cout << sums[count - 1] << "\n";
  delete[] sums;
}
