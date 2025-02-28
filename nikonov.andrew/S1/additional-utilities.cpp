#include "additional-utilities.hpp"
#include <iostream>

std::list< std::pair< std::string, std::list< int > > >* nikonov::getPList(std::istream& in)
{
  using pair_t = std::pair< std::string, std::list< int > >;
  std::list< pair_t >* stdList = new std::list< pair_t >;
  std::string data = "";
  std::string name = "";
  bool dataIsName = true;
  in >> data;
  while (!in.eof())
  {
    std::list< int > vals;
    while (!in.eof())
    {
      if (dataIsName)
      {
        name = data;
      }
      if (in >> data && std::isdigit(data[0]))
      {
        dataIsName = false;
        vals.push_back(std::stoi(data));
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
void nikonov::processPList(std::list< std::pair< std::string, std::list< int > > >* pList)
{
  auto pIter1 = pList->begin();
  size_t maxValCnt = (*pIter1).second.size();
  std::cout << (*pIter1).first;
  while (++pIter1 != pList->end())
  {
    std::cout << " " << (*pIter1).first;
    maxValCnt = maxValCnt > (*pIter1).second.size() ? maxValCnt : (*pIter1).second.size();
  }
  std::cout << '\n';
  size_t valId = 0;
  std::list< int > strSum;
  for (size_t i = 0; i < maxValCnt; ++i)
  {
    auto pIter2 = pList->begin();
    while (valId >= (*pIter2).second.size())
    {
      ++pIter2;
    }
    int currElem = *getIterAt((*(pIter2++)).second, valId);
    std::cout << currElem;
    size_t currSum = currElem;
    for (; pIter2 != pList->end(); ++pIter2)
    {
      if (valId < (*pIter2).second.size())
      {
        int currElem = *getIterAt((*pIter2).second, valId);
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

std::list< int >::iterator nikonov::getIterAt(std::list< int >& list, size_t id)
{
  auto iter = list.begin();
  for (size_t i = 0; i < id; ++i, ++iter);
  return iter;
}
