#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include "list_actions.hpp"


int main()
{
  try
  {
    std::list< pair_t > *sequences = evstyunichev::form_list(std::cin);
    evstyunichev::list_out(sequences, std::cout);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
