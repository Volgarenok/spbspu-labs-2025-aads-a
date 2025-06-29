#include <iostream>
#include <string>
#include <limits>
#include <list>

namespace
{
  using slop = std::list< std::pair< std::string, std::list< unsigned long long > > >;

  void printNums(std::ostream & out, const slop & listOfPairs)
  {
    size_t maxNums = listOfPairs.begin()->second.size();
    for (auto it = ++listOfPairs.begin(); it != listOfPairs.end(); ++it)
    {
      if (maxNums < it->second.size())
      {
        maxNums = it->second.size();
      }
    }
    std::list< unsigned long long > sumList;
    const unsigned long long max = std::numeric_limits< unsigned long long >::max();
    for (size_t i = 0; i < maxNums; i++)
    {
      unsigned long long sum = 0;
      auto it = listOfPairs.begin();
      auto currIt = it->second.begin();
      if (it->second.size() > i)
      {
        std::advance(currIt, i);
        out << *currIt;
        if (*currIt > max - sum)
        {
          throw std::overflow_error("overflow");
        }
        sum += *currIt;
      }
      else
      {
        while (it->second.size() <= i)
        {
          ++it;
        }
        currIt = it->second.begin();
        std::advance(currIt, i);
        out << *currIt;

        if (*currIt > max - sum)
        {
          throw std::overflow_error("overflow");
        }
        sum += *currIt;
      }
      ++it;
      for (; it != listOfPairs.end(); ++it)
      {
        auto currIt = it->second.begin();
        if (it->second.size() > i)
        {
          std::advance(currIt, i);
          out << " " << *currIt;
          if (*currIt > max - sum)
          {
            throw std::overflow_error("overflow");
          }
          sum += *currIt;
        }
      }
      sumList.push_back(sum);
      out << "\n";
    }
    out << *sumList.begin();
    for (auto it = ++sumList.begin(); it != sumList.end(); ++it)
    {
      out << " " << *it;
    }
    out << "\n";
  }
}

int main()
{
  std::list< std::pair< std::string, std::list< unsigned long long > > > listOfPairs;
  std::string nameList;
  while (std::cin >> nameList)
  {
    std::list< unsigned long long > numList;
    unsigned long long num = 0;
    while (std::cin >> num)
    {
      numList.push_back(num);
    }
    listOfPairs.push_back(std::make_pair(nameList, numList));
    std::cin.clear();
  }
  if (listOfPairs.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }
  std::cout << listOfPairs.begin()->first;
  for (auto it = ++listOfPairs.begin(); it != listOfPairs.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";
  try
  {
    printNums(std::cout, listOfPairs);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
