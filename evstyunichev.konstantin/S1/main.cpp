#include <iostream>
#include <iterator>
#include <list>
#include <string>

using pair_t = std::pair< std::string, std::list< int > >;


int main()
{
  std::list< pair_t > sequences{};
  std::list< long long > summ{};
  std::string str = "";
  bool non_empty_lists = false, overflow_flag = false;
  while (std::cin >> str)
  {
    if (isalpha(str[0]))
    {
      sequences.push_back({str, std::list< int >{}});
    }
    else
    {
      try
      {
        sequences.back().second.push_back(std::stoi(str));
        non_empty_lists = true;
      }
      catch(const std::out_of_range &e)
      {
        overflow_flag = 1;
        std::cerr << "overflow!\n";
        continue;
      }
    }
  }
  auto iter_seq = sequences.begin();
  if (!sequences.empty())
  {
    std::cout << (iter_seq++)->first;
    for (; iter_seq != sequences.end(); ++iter_seq)
    {
      std::cout << ' ' << iter_seq->first;
    }
  }
  if (!non_empty_lists)
  {
    summ.push_back(0);
    std::cout << '\n';
  }
  while (non_empty_lists)
  {
    std::cout << '\n';
    non_empty_lists = false;
    for (iter_seq = sequences.begin(); iter_seq != sequences.end(); ++iter_seq)
    {
      if (!(*iter_seq).second.empty())
      {
        non_empty_lists = true;
        int num = (*iter_seq).second.front();
        (*iter_seq).second.pop_front();
        std::cout << num;
        summ.push_back(num);
        ++iter_seq;
        break;
      }
    }
    for (; iter_seq != sequences.end(); ++iter_seq)
    {
      if ((*iter_seq).second.empty())
      {
        continue;
      }
      int num = (*iter_seq).second.front();
      std::cout << ' ' << num;
      summ.back() += num;
      non_empty_lists = true;
      (*iter_seq).second.pop_front();
    }
  }
  auto it = summ.begin();
  std::cout << *(it++);
  for (; it != summ.end(); it++)
  {
    std::cout << ' ' << *it;
  }
  std::cout << '\n';
  if (overflow_flag)
  {
    return 1;
  }
  return 0;
}
