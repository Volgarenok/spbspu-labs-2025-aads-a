#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "queue.hpp"
#include "stack.hpp"
#include "expr-utils.hpp"

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc > 2)
  {
    std::cerr << "ERROR: Too many arguments!\n";
    return 1;
  }

  std::istream* input = &std::cin;
  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file)
    {
      std::cerr << "ERROR: File error!\n";
      return 1;
    }
    input = &file;
  }

  Queue< Queue< std::string > > infixes;
  std::string line;
  while (std::getline(*input, line))
  {
    if (line.empty())
    {
      continue;
    }
    std::istringstream iss(line);
    std::string token;
    Queue< std::string > infix;
    while (iss >> token)
    {
      infix.push(token);
    }
    infixes.push(infix);
  }

  Queue< Queue< std::string > > postfixes;
  try
  {
    while (!infixes.empty())
    {
      Queue< std::string > infix = infixes.front();
      Queue< std::string > postfix = infixToPostfix(infix);
      infixes.drop();
      postfixes.push(postfix);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "ERROR: " << e.what() << "\n";
    return 1;
  }

  Stack< long long int > results;
  try
  {
    while (!postfixes.empty())
    {
      Queue< std::string > postfix = postfixes.drop();
      long long int result = 0ll;
      result = aleksandrov::evalPostfix(postfix);
      results.push(result);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "ERROR: " << e.what() << "\n";
    return 1;
  }

  if (!results.empty())
  {
    std::cout << results.drop();
  }
  while (!results.empty())
  {
    std::cout << " " << results.drop();
  }
  std::cout << "\n";
}

