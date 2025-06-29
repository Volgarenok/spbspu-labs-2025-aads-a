#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include "calculate_arithmetic_expressions.hpp"
#include "parse_text_and_push_it_to_queue.hpp"

namespace
{
  int checkArguments(const int argc)
  {
    if (argc == 2)
    {
      return 1;
    }
    else if (argc == 1)
    {
      return 2;
    }
    else
    {
      return 0;
    }
  }
}

int main(int argc, const char * const * argv)
{
  std::string text;
  std::queue< std::string > queue;
  if (!checkArguments(argc))
  {
    std::cerr << "ERROR: Too many arguments";
    std::cerr << "\n";
    return 1;
  }
  else if (checkArguments(argc) == 1)
  {
    std::ifstream input(argv[1]);
    while (input)
    {
      std::getline(input, text, '\n');
      if (input.eof())
      {
        break;
      }
      else if (text.empty())
      {
        continue;
      }
      else
      {
        petrov::parseTextAndPushItToQueue(text, queue);
      }
    }
  }
  else if (checkArguments(argc) == 2)
  {
    while (std::cin)
    {
      std::getline(std::cin, text);
      if (std::cin.eof())
      {
        break;
      }
      else if (text.empty())
      {
        continue;
      }
      else
      {
        petrov::parseTextAndPushItToQueue(text, queue);
      }
    }
  }
  if (queue.empty())
  {
    std::cout << "\n";
    return 0;
  }
  petrov::Stack< long long int > output_stack;
  try
  {
    output_stack = petrov::calculateArithmeticExpressions(queue);
  }
  catch (const std::out_of_range & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (const std::invalid_argument & e)
  {
    std::cerr << e.what() << "\n";
    return 2;
  }
  std::cout << output_stack.top();
  output_stack.pop();
  size_t stack_size = output_stack.size();
  for (size_t i = 0; i < stack_size; i++)
  {
    std::cout << " " << output_stack.top();
    output_stack.pop();
  }
  std::cout << "\n";
}
