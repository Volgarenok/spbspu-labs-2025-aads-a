#include <exception>
#include <fstream>
#include <iostream>
#include "action_calculator.hpp"

int main(int argc, char* argv[])
{
  using namespace brevnov;
  try
  {
    queue expression_index;
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      input_expression(file, expression_index);
    }
    else
    {
      input_expression(std::cin, expression_index);
    }
    queue postfix_expression = convert_expression(expression_index);
    stack_number results = calculation_expression(postfix_expression);
    output_results(std::cout, results) << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
