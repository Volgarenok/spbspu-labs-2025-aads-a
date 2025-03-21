#include <forward_list>
#include <iostream>
#include <utility>
#include "io_containers.hpp"

int main()
{
  zakirov::FwdList< std::pair< std::string, zakirov::FwdList< unsigned long long > > > forward_list;
  zakirov::get_list_pair(std::cin, forward_list);
  try
  {
    zakirov::output_result(std::cout, forward_list);
  }
  catch (const std::logic_error & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
