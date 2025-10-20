#include "actionsList.hpp"
#include <iterator>
#include <limits>
#include <stdexcept>

namespace
{
  size_t searchMax(const kiselev::list& list)
  {
    size_t max = 0;
    for (auto begin = list.cbegin(); begin != list.cend(); ++begin)
    {
      max = begin->second.size() == 0 ? max : std::max(max, begin->second.size());
    }
    return max;
  }

  unsigned long long checkedSum(unsigned long long sum, unsigned long long number)
  {
    const unsigned long long max = std::numeric_limits< unsigned long long >::max();
    if (max - number < sum)
    {
      throw std::overflow_error("Overflow for unsigned long long");
    }
    sum += number;
    return sum;
  }
}
kiselev::numberList kiselev::calculateSum(const list& list)
{
  auto it = list.cbegin();
  kiselev::numberList listSum;
  for (size_t i = 0; i < searchMax(list); ++i)
  {
    unsigned long long sum = 0;
    it = list.cbegin();
    for (; it != list.cend(); ++it)
    {
      auto nit = it->second.cbegin();
      if (i >= it->second.size())
      {
        continue;
      }
      std::advance(nit, i);
      sum = checkedSum(sum, *nit);
    }
    listSum.pushBack(sum);
  }
  return listSum;
}

std::ostream& kiselev::outputNumber(std::ostream& output, const numberList& list)
{
  output << list.front();
  for (auto it = ++list.cbegin(); it != list.cend(); ++it)
  {
    output << " " << *it;
  }
  return output;
}

std::ostream& kiselev::outputName(std::ostream& output, const list& list)
{
  output << list.front().first;
  for (auto it = ++list.cbegin(); it != list.cend(); ++it)
  {
    output << " " << it->first;
  }
  return output;
}

std::ostream& kiselev::outputNumbers(std::ostream& output, const list& list)
{
  auto it = list.cbegin();
  for (size_t i = 0; i < searchMax(list); ++i)
  {
    it = list.cbegin();
    kiselev::numberList numbers;
    for (; it != list.cend(); ++it)
    {
      auto nit = it->second.cbegin();
      if (i >= it->second.size())
      {
        continue;
      }
      std::advance(nit, i);
      numbers.pushBack(*nit);
    }
    outputNumber(output, numbers) << "\n";
  }
  return output;
}

kiselev::list kiselev::createList(std::istream& input)
{
  std::string name;
  list list;
  while (input >> name)
  {
    numberList numbers;
    unsigned long long number;
    while (input >> number)
    {
      if (!input)
      {
        throw std::logic_error("Incorrect number");
      }
      numbers.pushBack(number);
    }
    input.clear();
    list.pushBack(pair(name, numbers));
  }
  return list;
}

