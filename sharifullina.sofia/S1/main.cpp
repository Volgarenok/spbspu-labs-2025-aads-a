#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include "list.hpp"

namespace
{
  using listOfPairs = sharifullina::List< std::pair< std::string, sharifullina::List< unsigned long long > > >;

  size_t getMaxListSize(const listOfPairs& lists)
  {
    size_t maxSize = 0;
    for (const auto& pair : lists)
    {
      if (pair.second.size() > maxSize)
      {
        maxSize = pair.second.size();
      }
    }
    return maxSize;
  }

  void printTransposed(std::ostream& out, const listOfPairs & lists)
  {
    const size_t maxSize = getMaxListSize(lists);

    for (size_t i = 0; i < maxSize; ++i)
    {
      bool firstInRow = true;

      for (auto listIt = lists.begin(); listIt != lists.end(); ++listIt)
      {
        if (i < listIt->second.size())
        {
          auto numIt = listIt->second.begin();
          for (size_t j = 0; j < i; ++j)
          {
            ++numIt;
          }

          if (!firstInRow)
          {
            out << " ";
          }
          out << *numIt;
          firstInRow = false;
        }
      }
      out << "\n";
    }
  }

  void addWithCheck(unsigned long long & sum, unsigned long long value)
  {
    if (std::numeric_limits< unsigned long long >::max() - value < sum)
    {
      throw std::overflow_error("Overflow detected in sum calculation");
    }
    sum += value;
  }

  sharifullina::List< unsigned long long > calculateSums(const listOfPairs & lists)
  {
    sharifullina::List< unsigned long long > sums;
    const size_t maxSize = getMaxListSize(lists);

    for (size_t i = 0; i < maxSize; ++i)
    {
      unsigned long long rowSum = 0;

      for (auto listIt = lists.begin(); listIt != lists.end(); ++listIt)
      {
        if (i < listIt->second.size())
        {
          auto numIt = listIt->second.begin();
          for (size_t j = 0; j < i; ++j)
          {
            ++numIt;
          }
          addWithCheck(rowSum, *numIt);
        }
      }
      sums.pushBack(rowSum);
    }

    return sums;
  }

  void printSums(std::ostream& out, const sharifullina::List< unsigned long long > & sums)
  {
    if (sums.empty())
    {
      return;
    }

    auto it = sums.begin();
    out << *it;
    ++it;

    for (; it != sums.end(); ++it)
    {
      out << " " << *it;
    }
    out << "\n";
  }
}

int main()
{
  using namespace sharifullina;

  try
  {
    listOfPairs sequences;
    std::string name;

    while (std::cin >> name)
    {
      List< unsigned long long > numbers;
      unsigned long long number;

      while (std::cin >> number)
      {
        numbers.pushBack(number);
      }

      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

      if (!name.empty() && !numbers.empty())
      {
        sequences.pushBack(std::make_pair(name, numbers));
      }
    }

    if (sequences.empty())
    {
      std::cout << "0\n";
      return 0;
    }

    bool firstName = true;
    for (const auto & pair : sequences)
    {
      if (!firstName)
      {
        std::cout << " ";
      }
      std::cout << pair.first;
      firstName = false;
    }
    std::cout << "\n";

    printTransposed(std::cout, sequences);

    List< unsigned long long > sums = calculateSums(sequences);
    printSums(std::cout, sums);

    return 0;
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
