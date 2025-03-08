#include <iostream>
#include <iterator>
#include <string>
#include "../common/list.hpp"
#include "list_actions.hpp"


int main()
{
  evstyunichev::List< pair_t > *sequences = nullptr;
  try
  {
    sequences = evstyunichev::form_list(std::cin);
    evstyunichev::list_out(sequences, std::cout) << '\n';
    delete sequences;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    delete sequences;
    return 1;
  }
}
