#include <iostream>
#include "io_processing.hpp"

int main()
{
  // if (argc > 1)
  // {
  //   std::cerr << "\"One\" or \"None\" parameters expected, but " << argc << " are given!\n";
  //   return 1;
  // }
  // if (argc)
  // {
  //   return (argv != nullptr);
  //   //read standard input
  // }
  // //doing some file magic "Wshhhhh..Shhh..""
  nikonov::Queue< std::string > infixQueue = nikonov::readInfixExpressions(std::cin);
  nikonov::Stack< std::string > postfixStack;
  for(size_t i = 0; i < infixQueue.size(); ++i)
  {
    postfixStack.push(nikonov::convertToPostfix(infixQueue.back()));
    infixQueue.pop();
  }
  nikonov::printResults(postfixStack, std::cout);
}
