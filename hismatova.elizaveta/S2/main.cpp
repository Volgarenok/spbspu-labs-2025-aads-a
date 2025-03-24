#include <iostream>
#include <fstream>
#include "expression.hpp"
#include "stack.hpp"
#include "queue.hpp"
int main(int argc, char* argv[])
{
  Stack< int > results;
  std::istream* input = &std::cin;
  std::ifstream f;
  if (argc > 1)
  {
    f.open(argv[1]);
    if (!f)
    {
      std::cerr << "Error opening file\n";
      return 1;
    }
    input = &f;
  }
  std::string line;
  while (std::getline(*input, line))
  {
    if (line.empty())
    {
      continue;
    }
    try
    {
      Queue< PostfixValue > postfix = infixToPostfix(line);
      results.push(evaluatePostfix(postfix));
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << '\n';
      return 1;
    }
  }
  while (!results.empty())
  {
    std::cout << results.pop() << " ";
  }
  std::cout << '\n';
  return 0;
}
