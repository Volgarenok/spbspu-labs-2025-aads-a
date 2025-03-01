#include "additional-utilities.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>

std::list< std::pair< std::string, std::list< size_t > > > * nikonov::getPList(std::istream& in, bool& overflowFlag)
{
  using pair_t = std::pair< std::string, std::list< size_t > >;
  std::list< pair_t > * stdList = new std::list< pair_t >;
  std::string data = "";
  std::string name = "";
  bool dataIsName = true;
  in >> data;
  while (!in.eof())
  {
    std::list< size_t > vals;
    size_t currSum = 0;
    while (!in.eof())
    {
      if (dataIsName)
      {
        name = data;
      }
      if (in >> data && std::isdigit(data[0]))
      {
        dataIsName = false;
        size_t currElem = std::stoull(data);
        vals.push_back(currElem);
        if (currSum > std::numeric_limits< size_t >::max() - currElem)
        {
          overflowFlag = true;
        }
        currSum += currElem;
      }
      else
      {
        dataIsName = true;
        break;
      }
    }
    pair_t Pair{ name, vals };
    stdList->push_back(Pair);
  }
  return stdList;
}
void nikonov::processPList(std::list< std::pair< std::string, std::list< size_t > > > * pList, bool& overflowFlag)
{
  if (pList->size() == 0)
  {
    std::cout << 0 << '\n';
    return;
  }
  auto pIter1 = pList->begin();
  size_t maxValCnt = (*pIter1).second.size();
  std::cout << (*pIter1).first;
  while (++pIter1 != pList->end())
  {
    std::cout << " " << (*pIter1).first;
    maxValCnt = maxValCnt > (*pIter1).second.size() ? maxValCnt : (*pIter1).second.size();
  }
  std::cout << '\n';
  if (overflowFlag)
  {
    throw std::overflow_error("overflow detected");
  }
  size_t valId = 0;
  std::list< size_t > strSum;
  for (size_t i = 0; i < maxValCnt; ++i)
  {
    auto pIter2 = pList->begin();
    while (valId >= (*pIter2).second.size())
    {
      ++pIter2;
    }
    size_t currElem = *getIterAt((*(pIter2++)).second, valId);
    std::cout << currElem;
    size_t currSum = currElem;
    for (; pIter2 != pList->end(); ++pIter2)
    {
      if (valId < (*pIter2).second.size())
      {
        size_t currElem = *getIterAt((*pIter2).second, valId);
        currSum += currElem;
        std::cout << " " << currElem;
      }
    }
    strSum.push_back(currSum);
    ++valId;
    std::cout << '\n';
  }
  auto sumIter = strSum.begin();
  std::cout << *(sumIter++);
  for (size_t i = 1; i < strSum.size(); ++i)
  {
    std::cout << " " << *(sumIter++);
  }
  std::cout << '\n';
}

std::list< size_t >::iterator nikonov::getIterAt(std::list< size_t > & list, size_t id)
{
  auto iter = list.begin();
  for (size_t i = 0; i < id; ++i, ++iter);
  return iter;
}
