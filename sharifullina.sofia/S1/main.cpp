#include <iostream>
#include <string>
#include <limits>
#include <cstring>
#include <cctype>
#include "list.hpp"

namespace
{
  using listOfPairs = sharifullina::List< std::pair< std::string, sharifullina::List< unsigned long long > > >;

  size_t getMaxListSize(const listOfPairs & lists)
  {
    size_t maxSize = 0;
    for (listOfPairs::ConstIterator it = lists.begin(); it != lists.end(); ++it)
    {
      if (it->second.size() > maxSize)
      {
        maxSize = it->second.size();
      }
    }
    return maxSize;
  }

  void printNames(std::ostream & out, const listOfPairs & lists)
  {
    if (lists.empty())
    {
      return;
    }
    listOfPairs::ConstIterator it = lists.begin();
    out << it->first;
    ++it;
    for (; it != lists.end(); ++it)
    {
      out << " " << it->first;
    }
    out << "\n";
  }

  void printTransposed(std::ostream & out, const listOfPairs & lists)
  {
    size_t maxSize = getMaxListSize(lists);
    for (size_t i = 0; i < maxSize; ++i)
    {
      bool firstInRow = true;
      for (listOfPairs::ConstIterator listIt = lists.begin(); listIt != lists.end(); ++listIt)
      {
        if (i < listIt->second.size())
        {
          sharifullina::List< unsigned long long >::ConstIterator numIt = listIt->second.begin();
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
    size_t maxSize = getMaxListSize(lists);
    for (size_t i = 0; i < maxSize; ++i)
    {
      unsigned long long rowSum = 0;
      bool hasValue = false;
      for (listOfPairs::ConstIterator listIt = lists.begin(); listIt != lists.end(); ++listIt)
      {
        if (i < listIt->second.size())
        {
          sharifullina::List< unsigned long long >::ConstIterator numIt = listIt->second.begin();
          for (size_t j = 0; j < i; ++j)
          {
            ++numIt;
          }
          addWithCheck(rowSum, *numIt);
          hasValue = true;
        }
      }
      if (hasValue)
      {
        sums.pushBack(rowSum);
      }
    }
    return sums;
  }

  void printSums(std::ostream & out, const sharifullina::List< unsigned long long > & sums)
  {
    if (sums.empty())
    {
      out << "0\n";
      return;
    }
    sharifullina::List< unsigned long long >::ConstIterator it = sums.begin();
    out << *it;
    ++it;
    for (; it != sums.end(); ++it)
    {
      out << " " << *it;
    }
    out << "\n";
  }

  bool isNumber(const std::string & str)
  {
  if (str.empty())
    {
      return false;
    }
    const char digits[] = "0123456789";
    for (size_t i = 0; i < str.length(); ++i)
    {
      unsigned char c = static_cast< unsigned char >(str[i]);
      if (std::strchr(digits, c) == nullptr)
      {
        return false;
      }
    }
    return true;
  }

  unsigned long long parseNumber(const std::string & str)
  {
    unsigned long long result = 0;
    const char digits[] = "0123456789";
    for (size_t i = 0; i < str.length(); ++i)
    {
      unsigned char c = static_cast< unsigned char >(str[i]);
      const char * pos = std::strchr(digits, c);
      if (pos == nullptr)
      {
        throw std::invalid_argument("Invalid digit in number");
      }
      unsigned long long digit = pos - digits;
      if (result > (std::numeric_limits< unsigned long long >::max() - digit) / 10)
      {
        throw std::overflow_error("Number too large");
      }
      result = result * 10 + digit;
    }
    return result;
  }
}
int main()
{
  using namespace sharifullina;

  try
  {
    listOfPairs sequences;
    std::string line;

    while (std::getline(std::cin, line))
    {
      if (line.empty())
      {
         continue;
      }
      size_t pos = 0;
      while (pos < line.length() && line[pos] == ' ')
      {
        pos++;
      }
      if (pos >= line.length())
      {
        continue;
      }
      size_t nameStart = pos;
      while (pos < line.length() && line[pos] != ' ')
      {
        pos++;
      }
      std::string name = line.substr(nameStart, pos - nameStart);
      List< unsigned long long > numbers;
      while (pos < line.length())
      {
        while (pos < line.length() && line[pos] == ' ')
        {
          pos++;
        }
        if (pos >= line.length())
        {
          break;
        }
        size_t wordStart = pos;
        while (pos < line.length() && line[pos] != ' ')
        {
          pos++;
        }
        std::string word = line.substr(wordStart, pos - wordStart);
        if (isNumber(word))
        {
          unsigned long long number = parseNumber(word);
          numbers.pushBack(number);
        }
        else
        {
          break;
        }
      }

      sequences.pushBack(std::make_pair(name, numbers));
    }

    if (sequences.empty())
    {
      std::cout << "0\n";
      return 0;
    }

    printNames(std::cout, sequences);
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
