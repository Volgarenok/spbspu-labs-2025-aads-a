#include <exception>
#include <fstream>
#include <iostream>
#include "action_calculator.hpp"

int main(int argc, char* argv[])
{
  try
  {
    brevnov::queue example_index;
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      brevnov::input_example(file, example_index);
    }
    else
    {
      brevnov::input_example(std::cin, example_index);
    }
    brevnov::queue postfix_example = brevnov::convert_example(example_index);
    brevnov::stack_number results = brevnov::calculation_example(postfix_example);
    brevnov::output_results(std::cout, results) << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
