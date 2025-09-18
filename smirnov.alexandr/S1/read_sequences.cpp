#include "read_sequences.hpp"
#include <cctype>
#include <stdexcept>
#include <list/list.hpp>
#include <list/list_utils.hpp>

void smirnov::readSequences(std::istream & in, List< std::pair< std::string, List< size_t > > > & sequences)
{
  std::string input;
  std::string name;
  List< size_t > list;
  while (in >> input)
  {
    if (std::isdigit(input[0]))
    {
      try
      {
        size_t number = std::stoull(input);
        pushBack(list, number);
      }
      catch (const std::out_of_range & e)
      {
        throw std::runtime_error("Number is too large");
      }
    }
    else
    {
      if (!name.empty())
      {
        pushBack(sequences, std::make_pair(name, list));
        list.clear();
      }
      name = input;
    }
  }

  if (!name.empty())
  {
    pushBack(sequences, std::make_pair(name, list));
  }
}

void smirnov::newSequences(List< List< size_t > > & out, List< std::pair< std::string, List< size_t > > > & in)
{
  size_t maxLength = 0;
  for (auto it = in.begin(); it != in.end(); ++it)
  {
    if (it->second.size() > maxLength)
    {
      maxLength = it->second.size();
    }
  }
  for (size_t i = 0; i < maxLength; ++i)
  {
    List< size_t > numbers;
    for (auto it = in.begin(); it != in.end(); ++it)
    {
      if (i < it->second.size())
      {
        auto numIt = it->second.begin();
        for (size_t j = 0; j < i; ++j)
        {
          ++numIt;
        }
        pushBack(numbers, *numIt);
      }
    }
    if (!numbers.empty())
    {
      pushBack(out, numbers);
    }
  }
}

