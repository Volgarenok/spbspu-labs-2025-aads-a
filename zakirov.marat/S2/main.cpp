#include <iostream>
#include <fstream>
#include "infix_to_postfix.hpp"
#include "io_stack.hpp"
#include "stack.hpp"
#include "queue.hpp"

int main(int argc, char ** argv)
{
  if (argc > 2 || argc < 1)
  {
    std::cerr << "Incorrect command line argument" << '\n';
    return 1;
  }

  zakirov::Stack< zakirov::Queue < std::string > > stack_qs;
  if (argc == 1)
  {
    zakirov::scan_infix(std::cin, stack_qs);
  }
  else if (argc == 2)
  {
    std::ifstream file(argv[1]);
    if (!file)
    {
      std::cerr << "Critical error in file" << '\n';
      return 1;
    }

    zakirov::scan_infix(file, stack_qs);
  }

  if (stack_qs.empty())
  {
    std::cout <<'\n';
    return 0;
  }

  std::cout << zakirov::calculate_postfix_expression(transform_to_postfix(stack_qs.top()));
  stack_qs.pop();
  while (!stack_qs.empty())
  {
    std::cout << ' ' << zakirov::calculate_postfix_expression(stack_qs.top());
    stack_qs.pop();
  }
  std::cout << '\n';
}
