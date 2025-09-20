#include <iostream>
#include <fstream>
#include <string>
#include "calculationprocess.hpp"
#include "queue.hpp"
#include "stack.hpp"

namespace
{
  void splitExpr(const std::string& string, kushekbaev::Queue< std::string >& arithmeticQ)
  {
    std::string symbol;
    char delimiter = ' ';
    size_t begin = string.find_first_not_of(delimiter);
    size_t end = 0;
    while (begin != std::string::npos)
    {
      end = string.find(delimiter, begin);
      symbol = string.substr(begin, end - begin);
      arithmeticQ.push(symbol);
      begin = string.find_first_not_of(delimiter, end);
    }
  }

  void inputExpr(std::istream& input, kushekbaev::Queue< kushekbaev::Queue< std::string > >& Q)
  {
    std::string string;
    kushekbaev::Queue< std::string > infixQ;
    while (std::getline(input, string))
    {
      if (string.empty())
      {
        continue;
      }
      infixQ.clear();
      splitExpr(string, infixQ);
      Q.push(std::move(infixQ));
    }
  }

  void output(std::ostream& output, kushekbaev::Stack< long long int >& results)
  {
    if (!results.empty())
    {
      output << results.top();
      results.pop();
      while (!results.empty())
      {
        output << " " << results.top();
        results.pop();
      }
    }
  }
}

int main(int argc, char* argv[])
{
  using namespace kushekbaev;
  Queue< Queue< std::string > > inputQ;
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
      inputExpr(filename, inputQ);
    }
    else
    {
      inputExpr(std::cin, inputQ);
    }
    while (!inputQ.empty())
    {
      Queue< std::string > infixQ = inputQ.front();
      inputQ.pop();
      Queue< std::string > postfixQ = convertToPostfix(infixQ);
      long long int result = calculatePostfix(postfixQ);
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
