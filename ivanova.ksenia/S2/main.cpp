#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <string>

#include "expression.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::cerr << "usage: " << argv[0] << " [filename]\n";
    return 1;
  }

  std::ifstream fin;
  if (argc == 2)
  {
    fin.open(argv[1]);
    if (!fin.is_open())
    {
      std::cerr << "can't open file '" << argv[1] << "'\n";
      return 1;
    }
  }
  std::istream& input = fin.is_open() ? fin : std::cin;

  ivanova::Stack<int64_t> result;
  try
  {
    std::string line;
    while (std::getline(input, line))
    {
      if (line.empty())
      {
        continue;
      }
      int64_t value = ivanova::countExpression(line);
      result.push(value);
    }
  }
  catch (std::logic_error& ex)
  {
    std::cerr << ex.what() << "\n";
    return 1;
  }

  if (!result.empty())
  {
    std::cout << result.top();
    result.pop();
  }
  while (!result.empty())
  {
    std::cout << " " << result.top();
    result.pop();
  }
  std::cout << "\n";

  return 0;
}
