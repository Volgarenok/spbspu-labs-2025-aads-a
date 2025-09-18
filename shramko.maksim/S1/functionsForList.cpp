#include "functionsForList.hpp"

void shramko::printNames(const PairList& lists, std::ostream& out)
{
  if (lists.isEmpty())
  {
    return;
  }
  bool isFirst = true;
  for (auto it = lists.begin(); it != lists.end(); ++it)
  {
    if (!isFirst)
    {
      out << " ";
    }
    out << it->first;
    isFirst = false;
  }
  out << "\n";
}

bool shramko::checkSumOverflow(int a, int b)
{
  return (b > std::numeric_limits< int >::max() - a);
}

void shramko::printSumResult(const ForwardList< int >& sums, bool overflow, std::ostream& out)
{
  if (overflow)
  {
    return;
  }

  if (sums.isEmpty())
  {
    out << "0\n";
    return;
  }

  bool isFirst = true;
  for (auto it = sums.begin(); it != sums.end(); ++it)
  {
    if (!isFirst)
    {
      out << " ";
    }
    out << *it;
    isFirst = false;
  }
  out << "\n";
}

void shramko::processLists(const PairList& lists, size_t maxLen, bool& overflow, std::ostream& out)
{
  ForwardList< int > sumList;

  for (size_t i = 0; i < maxLen; ++i)
  {
    int currentSum = 0;
    bool firstElement = true;

    for (const auto& pair: lists)
    {
      if (i < pair.second.getSize())
      {
        auto it = pair.second.begin();
        for (size_t j = 0; j < i; ++j)
        {
          ++it;
        }

        if (*it > static_cast< unsigned long long >(std::numeric_limits< int >::max()))
        {
          overflow = true;
        }
        else
        {
          int currentValue = static_cast< int >(*it);
          if (checkSumOverflow(currentSum, currentValue))
          {
            overflow = true;
          }
          else
          {
            currentSum += currentValue;
          }
        }

        if (!firstElement)
        {
          out << " ";
        }
        out << *it;
        firstElement = false;
      }
    }
    out << "\n";
    sumList.addToBack(currentSum);
  }

  printSumResult(sumList, overflow, out);
}
