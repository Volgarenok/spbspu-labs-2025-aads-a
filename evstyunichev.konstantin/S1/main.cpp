#include <iostream>
#include <iterator>
#include <list>
#include <string>

using pair_t = std::pair< std::string, std::list< int > >;


int main()
{
  std::list< pair_t > sequences;
  std::list< long long > summ;
  std::string str;
  bool flag = false;
  while (std::cin >> str)
  {
    if (isalpha(str[0]))
    {
      sequences.push_back({str, std::list< int >{}});
    }
    else
    {
      sequences.back().second.push_back(std::stoi(str));
      flag = true;
    }
  }
  auto iter_seq = sequences.begin();
  std::cout << (iter_seq++)->first;
  for (; iter_seq != sequences.end(); ++iter_seq)
  {
    std::cout << ' ' << iter_seq->first;
  }
  if (!flag)
  {
    std::cout << '\n';
  }
  while (flag)
  {
    std::cout << '\n';
    flag = false;
    for (iter_seq = sequences.begin(); iter_seq != sequences.end(); ++iter_seq)
    {
      if (!(*iter_seq).second.empty())
      {
        if (!flag)
        {
          summ.push_back(0ll);
        }
        int num = (*iter_seq).second.front();
        std::cout << num << ' ';
        summ.back() += num;
        flag = true;
        (*iter_seq).second.pop_front();
      }
    }
  }
  auto it = summ.begin();
  std::cout << *(it++);
  for (; it != summ.end(); it++)
  {
    std::cout << ' ' << *it;
  }
  std::cout << '\n';
  return 0;
}
