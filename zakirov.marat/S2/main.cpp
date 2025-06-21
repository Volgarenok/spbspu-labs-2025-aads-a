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
  if (argc == 1)
  {
    while (std::cin)
    {
      getline(std::cin, reader);
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
  }
  else if (argc == 2)
  {
    std::ifstream file(argv[1]);
    while (file)
    {
      getline(file, reader);
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
  catch (const std::invalid_argument & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  catch (const std::overflow_error & e)
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
    catch (const std::invalid_argument & e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
    catch (const std::overflow_error & e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }

    stack_qs.pop();
  }

  std::cout << '\n';
}
