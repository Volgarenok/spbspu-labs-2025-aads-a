#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include "../common/list.hpp"
#include "list_actions.hpp"


int main()
{
  using pair_t = std::pair< std::string, evstyunichev::List< size_t> >;
  evstyunichev::List< int > list{};
  try
  {
    std::string str = "";
    evstyunichev::List< pair_t > sequences{};
    while (std::cin >> str)
    {
      if (isalpha(str[0]))
      {
        sequences.push_back({ str, evstyunichev::List< size_t >{} });
      }
      else
      {
        size_t n = std::stoull(str);
        sequences.back().second.push_back(n);
      }
    }
    evstyunichev::list_out(sequences, std::cout) << '\n';
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
