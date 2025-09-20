#include <iostream>
#include "output.hpp"
#include "input.hpp"
#include "fwd_list.hpp"
#include "count_sum.hpp"

int main()
{
  std::string name = "kekw";
  karnauhova::FwdList< std::pair< std::string, karnauhova::FwdList< unsigned long long > > > lists;
  try
  {
    while (std::cin >> name && !std::cin.eof())
    {
      lists.push_front(karnauhova::input_line(std::cin, name));
    }
    lists.reverse();
    karnauhova::output_names(lists, std::cout);
    karnauhova::output_lists(lists, std::cout);
    karnauhova::count_lists(lists, std::cout);
  }
  catch (const std::logic_error& e)
  {
    std::cerr << "Overflow\n";
    return 1;
  }
}
