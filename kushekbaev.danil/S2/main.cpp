#include <iostream>
#include "io-utils.hpp"
#include "calculationProcess.hpp"

int main(int argc, char* argv[])
{
  std::string filename;
  if (argc > 1)
  {
    filename = argv[1];
  }
  kushekbaev::Queue< kushekbaev::Queue< std::string > > Q;
  kushekbaev::Stack< long long int > results;
  try
  {
    kushekbaev::openFile(filename, Q);
    while (!Q.empty())
    {
      kushekbaev::Queue< std::string > infixQ = Q.front();
      Q.pop();
      kushekbaev::Queue< std::string > postfixQ = kushekbaev::convertToPostfix(infixQ);
      long long int result = kushekbaev::calculatePostfix(postfixQ);
      results.push(result);
    }
    kushekbaev::output(std::cout, results);
    std::cout << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
