#include <iostream>
#include <fstream>
#include <string>
#include "calculationprocess.hpp"
#include "ioutils.hpp"
#include "queue.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
  using namespace kushekbaev;
  Queue< Queue< std::string > > inputQueue;
  Stack< long long int > results;
  try
  {
    if (argc > 1)
    {
      std::ifstream filename(argv[1]);
      if (!filename.is_open())
      {
        std::cerr << "The file couldn't be opened!";
        return 1;
      }
      inputExpr(filename, inputQueue);
    }
    else
    {
      inputExpr(std::cin, inputQueue);
    }
    while (!inputQueue.empty())
    {
      Queue< std::string > infixQueue = inputQueue.front();
      inputQueue.pop();
      Queue< std::string > postfixQueue = convertToPostfix(infixQueue);
      long long int result = calculatePostfix(postfixQueue);
      results.push(result);
    }
    output(std::cout, results);
    std::cout << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
