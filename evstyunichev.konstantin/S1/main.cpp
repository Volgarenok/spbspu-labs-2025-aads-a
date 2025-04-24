#include <iostream>
#include <iterator>
#include <string>
#include "../common/list.hpp"
#include "list_actions.hpp"


int main()
{
  evstyunichev::List< int > list{};
  /*
  try
  {
    evstyunichev::List< pair_t > sequences{};
    std::string str = "";
    while (std::cin >> str)
    {
      if (isalpha(str[0]))
      {
        sequences.push_back({str, evstyunichev::List< size_t >{} });
      }
      else
      {
        size_t n = std::stoull(str);
        sequences.back().second.push_back(n);
      }
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  */
}
