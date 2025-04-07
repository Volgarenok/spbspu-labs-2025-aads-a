#include <iostream>
#include <fstream>
#include "io_processing.hpp"
#include "postfix.hpp"
int main(int argc, char ** argv)
{
  nikonov::Queue< std::string > infixQueue;
  if (argc > 2)
  {
    std::cerr << "\"One\" or \"None\" parameters expected, but " << argc - 1 << " are given!\n";
    return 2;
  }
  else if (argc == 1)
  {
    infixQueue = nikonov::readInfixExpressions(std::cin);
  }
  else if (argc == 2)
  {
    std::ifstream in(argv[1]);
    if (!in.is_open())
    {
      std::cerr << "Can't access the file\n";
      return 1;
    }
    infixQueue = nikonov::readInfixExpressions(in);
    in.close();
  }
  nikonov::Stack< nikonov::Postfix > postfixStack;
  try
  {
    while (!infixQueue.empty())
    {
      postfixStack.push(nikonov::Postfix(infixQueue.front(), 0));
      infixQueue.pop();
    }
  }
  catch(const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  nikonov::printResults(postfixStack, std::cout);
}
