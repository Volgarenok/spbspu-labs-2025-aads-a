#include "read_sequences.hpp"
#include <cctype>
#include <list>
#include <stdexcept>

void smirnov::readSequences(std::istream & in, std::list< std::pair< std::string, std::list< size_t > > > & sequences)
{
  std::string input;
  std::string name;
  std::list< size_t > list;
  while (in >> input)
  {
    if (std::isdigit(input[0]))
    {
      try
      {
        size_t number = std::stoull(input);
        list.push_back(number);
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
        sequences.push_back(std::make_pair(name, list));
        list.clear();
      }
      name = input;
    }
  }

  if (!name.empty())
  {
    sequences.push_back(std::make_pair(name, list));
  }
}

void smirnov::newSequences(std::list< std::list< size_t > > & out, std::list< std::pair< std::string, std::list< size_t > > > & in)
{
  bool hasNumbers = true;
  while (hasNumbers)
  {
    hasNumbers = false;
    std::list< size_t > resultSequence;
    for (auto it = in.begin(); it != in.end(); ++it)
    {
      if (!it->second.empty())
      {
        resultSequence.push_back(it->second.front());
        it->second.pop_front();
        hasNumbers = true;
      }
    }
    if (!resultSequence.empty())
    {
      out.push_back(resultSequence);
    }
  }
}

