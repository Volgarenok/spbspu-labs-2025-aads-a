#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include "list_actions.hpp"


int main()
{
  std::list< pair_t > *sequences = nullptr;
  try
  {
    sequences = evstyunichev::form_list(std::cin);
    evstyunichev::list_out(sequences, std::cout) << '\n';
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
