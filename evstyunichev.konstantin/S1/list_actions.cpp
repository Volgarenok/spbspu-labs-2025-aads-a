#include "list_actions.hpp"
#include <iostream>
#include <list>
#include <stdexcept>

std::list< pair_t > *evstyunichev::form_list(std::istream &in)
{
  std::list< pair_t > *sequences = new std::list< pair_t >;
  std::string str = "";
  while (in >> str)
  {
    if (isalpha(str[0]))
    {
      sequences->push_back({str, std::list< int >{}});
    }
    else
    {
      sequences->back().second.push_back(std::stoi(str));
    }
  }
  if (sequences->empty())
  {
    throw std::logic_error("empty!");
  }
  return sequences;
}

int list_out(std::list< pair_t > *sequences, std::ostream &out)
{
  std::list< long long > summ{};
  size_t mx = 0;
  if (!sequences->empty())
  {
    auto iter_seq = sequences->begin();
    std::cout << (iter_seq++)->first;
    for (; iter_seq != sequences->end(); ++iter_seq)
    {
      std::cout << ' ' << iter_seq->first;
      mx = (iter_seq->second.size() > mx ? iter_seq->second.size() : mx);
    }
  }
  if (mx && !sequences->empty())
  {
    std::cout << '\n';
    summ.push_back(0);
  }
  while (mx--)
  {
    auto iter_seq = sequences->begin();
    out << '\n';
    for (iter_seq = sequences->begin(); iter_seq != sequences->end(); ++iter_seq)
    {
      if (!iter_seq->second.empty())
      {
        int num = iter_seq->second.front();
        iter_seq->second.pop_front();
        out << num;
        summ.push_back(num);
        ++iter_seq;
        break;
      }
    }
    for (; iter_seq != sequences->end(); ++iter_seq)
    {
      if (iter_seq->second.empty())
      {
        continue;
      }
      int num = iter_seq->second.front();
      out << ' ' << num;
      summ.back() += num;
      iter_seq->second.pop_front();
    }
  }
  auto it = summ.begin();
  out << *(it++);
  for (; it != summ.end(); it++)
  {
    out << ' ' << *it;
  }
  return 0;
}
