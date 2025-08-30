#include <string>
#include <iostream>
#include <limits>
#include "forward_ring_list.hpp"

namespace petrov
{
  using pair_of_string_and_list = std::pair< std::string, petrov::ForwardRingList< size_t > >;
  using list_type = ForwardRingList< pair_of_string_and_list >;
  using list_it_t = ForwardListIterator< pair_of_string_and_list >;
  using sublist_it_t = ForwardListIterator< size_t >;
  using result_list_type = ForwardRingList< size_t >;
  std::istream & inputValuesIntoFwdRingList(std::istream & in, list_type & fwd_ring_list);
  std::ostream & outputNamesOfSuquences(std::ostream & out, const list_type & fwd_ring_list);
  result_list_type getListOfSequencesFromListOfSums(list_type fwd_ring_list);
  void addElementToSumAndPopFrontIt(size_t & sum, list_it_t it_out, sublist_it_t it);
  std::ostream & outputSums(std::ostream & out, const result_list_type sums);
}

int main()
{
  using namespace petrov;
  list_type fwd_ring_list = {};
  result_list_type sums = {};
  try
  {
    inputValuesIntoFwdRingList(std::cin, fwd_ring_list);
    if (fwd_ring_list.empty())
    {
      std::cout << 0;
      std::cout << "\n";
      return 0;
    }
    outputNamesOfSuquences(std::cout, fwd_ring_list);
    sums = getListOfSequencesFromListOfSums(fwd_ring_list);
  }
  catch (const std::exception & e)
  {
    std::cout << "\n";
    std::cerr << e.what() << "\n";
    return 1;
  }
  sums.reverse();
  if (sums.empty())
  {
    std::cout << 0;
  }
  else
  {
    outputSums(std::cout, sums);
  }
  std::cout << "\n";
}

std::istream & petrov::inputValuesIntoFwdRingList(std::istream & in, list_type & fwd_ring_list)
{
  std::string sequence_num = {};
  size_t number = 0;
  while (!in.eof())
  {
    in.clear();
    in >> sequence_num;
    if (sequence_num.empty())
    {
      break;
    }
    petrov::ForwardRingList< size_t > sublist = {};
    while (!in.eof() && in)
    {
      in >> number;
      if (!in)
      {
        break;
      }
      sublist.push_front(number);
    }
    sublist.reverse();
    pair_of_string_and_list new_pair = { sequence_num, sublist };
    fwd_ring_list.push_front(new_pair);
  }
  fwd_ring_list.reverse();
  return in;
}

std::ostream & petrov::outputNamesOfSuquences(std::ostream & out, const list_type & fwd_ring_list)
{
  auto it = fwd_ring_list.cbegin();
  if (fwd_ring_list.size() == 1)
  {
    out << it->first;
  }
  else
  {
    out << (it++)->first;
    do
    {
      out << " " << it->first;
    }
    while (it++ != fwd_ring_list.cend());
  }
  out << "\n";
  return out;
}

petrov::result_list_type petrov::getListOfSequencesFromListOfSums(list_type fwd_ring_list)
{
  result_list_type sums = {};
  do
  {
    size_t sum = 0;
    auto prev_it_out = fwd_ring_list.end();
    auto it_out = fwd_ring_list.begin();
    if (it_out->second.empty() && fwd_ring_list.size() == 1)
    {
      fwd_ring_list.pop_front();
      break;
    }
    else
    {
      if (it_out->second.empty() && fwd_ring_list.size() != 1)
      {
        auto val = *it_out;
        fwd_ring_list.remove(val);
        it_out = prev_it_out;
      }
      else
      {
        auto it = it_out->second.begin();
        std::cout << *it;
        addElementToSumAndPopFrontIt(sum, it_out, it);
        ++prev_it_out;
      }
      while (it_out++ != fwd_ring_list.end())
      {
        if (it_out->second.empty() && fwd_ring_list.size() != 1)
        {
          auto val = *it_out;
          fwd_ring_list.remove(val);
          it_out = prev_it_out;
        }
        else
        {
          auto it = it_out->second.begin();
          std::cout << " " << *it;
          addElementToSumAndPopFrontIt(sum, it_out, it);
          ++prev_it_out;
        }
      }
    }
    if (sum)
    {
      sums.push_front(sum);
      std::cout << "\n";
    }
  }
  while (!fwd_ring_list.empty());
  return sums;
}

void petrov::addElementToSumAndPopFrontIt(size_t & sum, list_it_t it_out, sublist_it_t it)
{
  if (sum <= std::numeric_limits< size_t >::max() - *it)
  {
    sum += *it;
    it_out->second.pop_front();
  }
  else
  {
    throw std::out_of_range("ERROR: Out of range");
  }
}

std::ostream & petrov::outputSums(std::ostream & out, const result_list_type sums)
{
  if (sums.size() == 1)
  {
    out << *sums.cbegin();
  }
  else
  {
    auto it = sums.cbegin();
    out << *(it++);
    do
    {
      out << " " << *it;
    }
    while (it++ != sums.cend());
  }
  return out;
}

