#include <iostream>
#include <fstream>
#include <string>
#include "queue.hpp"
#include "stack.hpp"
#include "calculatorExpression.hpp"
#include "ioExpression.hpp"

int main(int argc, char ** argv)
{
  using namespace maslov;
  std::string filename;
  if (argc > 1)
  {
    filename = argv[1];
  }
  Queue< Queue< std::string > > queue;
  Stack< long long int > results;
  try
  {
    inputFile(filename, queue);
    while (!queue.empty())
    {
      Queue< std::string > infixQueue = queue.front();
      queue.pop();
      Queue< std::string > postfixQueue = infixToPostfix(infixQueue);
      long long int result = calculatePostfix(postfixQueue);
      results.push(result);
    }
    printData(std::cout, results);
    std::cout << "\n";
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
