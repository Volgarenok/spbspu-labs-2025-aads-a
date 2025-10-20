#include "io-utils.hpp"

std::istream& kizhin::inputSequences(std::istream& in, NamesT& names,
    ForwardList< NumbersT >& numbersLists)
{
  NamesT::value_type currName;
  NumbersT currNums;
  while (in >> currName) {
    names.pushBack(currName);
    using IstremIterT = std::istream_iterator< NumbersT::value_type >;
    currNums.assign(IstremIterT(in), IstremIterT{});
    numbersLists.pushBack(currNums);
    in.clear();
  }
  in.clear();
  return in;
}

