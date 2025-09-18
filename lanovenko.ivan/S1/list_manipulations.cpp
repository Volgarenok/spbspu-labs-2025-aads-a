#include "list_manipulations.hpp"

void lanovenko::printSequencesName(const list_of_pairs& temporary, std::ostream& out)
{
  if (temporary.empty())
  {
    return;
  }
  auto it = temporary.begin();
  out << (*it).first;
  for (++it; it != temporary.end(); it++)
  {
    out << " " << (*it).first;
  }
  out << '\n';
}

bool lanovenko::isSumLimit(size_t a, size_t b)
{
  return (b > std::numeric_limits< int >::max() - a);
}

void lanovenko::printSumList(const ForwardList< int >& sumList, bool sumLimit, std::ostream& out)
{
  if (sumLimit)
  {
    return;
  }
  if (!sumList.empty())
  {
    auto it = sumList.begin();
    out << (*it);
    for (++it; it != sumList.end(); it++)
    {
      out << " " << (*it);
    }
  }
  else
  {
    out << 0;
  }
  out << '\n';
}

void lanovenko::printRowsLists(const ForwardList< list_ull >& rowsLists, std::ostream& out)
{
  for (auto i = rowsLists.begin(); i != rowsLists.end(); i++)
  {
    auto j = (*i).begin();
    out << (*j);
    for (++j; j != (*i).end(); j++)
    {
      out << ' ' << (*j);
    }
    out << '\n';
  }
}

void lanovenko::printSequences(const list_of_pairs& temporary, size_t maxSize, bool& sumLimit, std::ostream& out)
{
  ForwardList< int > sumList{};
  ForwardList< list_ull > rowsList;
  for (size_t i = 0; i < maxSize; i++)
  {
    size_t res = 0;
    list_ull current;
    for (const auto& pair: temporary)
    {
      if (!pair.second.empty() && i < pair.second.size())
      {
        auto it = pair.second.begin();
        std::advance(it, i);
        current.push_back(*it);
        if (isSumLimit(res, *it))
        {
          sumLimit = true;
        }
        res += *(it);
      }
    }
    sumList.push_back(res);
    if (!current.empty())
    {
      rowsList.push_back(current);
    }
  }
  printRowsLists(rowsList, out);
  printSumList(sumList, sumLimit, out);
}
