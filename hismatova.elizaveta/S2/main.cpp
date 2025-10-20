#include <iostream>
#include <fstream>
#include <vector>
#include "expression.hpp"
#include "stack.hpp"
int main(int argc, char* argv[])
{
  using namespace hismatova;
  std::vector< long long > results;
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
      results.push_back(evaluatePostfix(postfix));
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << '\n';
      return 1;
    }
  }
  for (int i = results.size() - 1; i >= 0; --i)
  {
    std::cout << results[i];
    if (i != 0)
    {
      std::cout << " ";
    }
  }
  std::cout << '\n';
  return 0;
}
