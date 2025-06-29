#include "input_outputLists.hpp"

std::istream &abramov::inputLists(std::istream &in, list_of_lists lists, size_t &lists_count, size_t &count)
{
  std::string s;
  in >> s;
  while (in)
  {
    std::pair< std::string, List< unsigned long long int > > bidir;
    bidir.first = s;
    in >> s;
    while(in && std::isdigit(s[0]))
    {
      bidir.second.pushBack(std::strtoull(s.c_str(), nullptr, 10));
      in >> s;
    }
    lists->pushBack(bidir);
    count = std::max(count, bidir.second.size());
    ++lists_count;
  }
  return in;
}

std::ostream &abramov::printNames(std::ostream &out, const list_of_lists lists)
{
  for (auto iter = lists->begin(); iter != --lists->end(); ++iter)
  {
    out << iter->first << " ";
  }
  out << (--lists->end())->first << "\n";
  return out;
}

std::ostream &abramov::printValues(std::ostream &out, const list_of_lists lists, size_t count)
{
  for (size_t i = 0; i < count; ++i)
  {
    bool flag = false;
    for (auto iter = lists->begin(); iter != --lists->end(); ++iter)
    {
      if (iter->second.size() > i)
      {
        auto iter2 = std::next(iter->second.begin(), i);
        if (flag)
        {
          out << " ";
        }
        out << *iter2;
        flag = true;
      }
    }
    if ((--lists->end())->second.size() <= i)
    {
      out << "\n";
    }
    else
    {
      out << " " << *(std::next((--lists->end())->second.begin(), i)) << "\n";
    }
  }
  return out;
}
