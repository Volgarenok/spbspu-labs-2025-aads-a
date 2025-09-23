#include <iostream>
#include <fstream>
#include "stack.hpp"
#include "queue.hpp"
#include "postfix.hpp"

int main(int argc, char ** argv)
{
  if (argc > 2 || argc < 1)
  {
    std::cerr << "Incorrect command line argument" << '\n';
    return 1;
  }

  zakirov::Stack< zakirov::Postfix > stack_qs;
  std::string reader;
  std::fstream file;
  std::istream & input_s = (argc == 1) ? std::cin : (file.open(argv[1]), file);

  while (input_s)
  {
    std::getline(input_s, reader);
    if (!reader.empty())
    {
      try
      {
        stack_qs.push(zakirov::Postfix(reader));
      }
      catch (const std::invalid_argument & e)
      {
        std::cerr << e.what() << '\n';
        return 1;
      }
    }
  }

  if (stack_qs.empty())
  {
    std::cout << '\n';
    return 0;
  }

  try
  {
    std::cout << stack_qs.top()();
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  stack_qs.pop();
  while (!stack_qs.empty())
  {
    try
    {
      std::cout << ' ' << stack_qs.top()();
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }

    stack_qs.pop();
  }

  std::cout << '\n';
}
