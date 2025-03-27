#include "io-utils.hpp"
#include <iostream>
#include <fstream>

namespace
{
  void splitExpr(std::string string, kushekbaev::Queue< std::string >& arithmeticQ)
  {
    std::string symbol;
    const char* space = " ";
    for (size_t i = 0; i < string.length(); ++i)
    {
      if (string[i] != *space)
      {
        symbol += string[i];
      }
      else if (!symbol.empty())
      {
        arithmeticQ.push(symbol);
        symbol.clear();
      }
    }
    if (!symbol.empty())
    {
      arithmeticQ.push(symbol);
    }
  }

  void inputExpr(std::istream& input, kushekbaev::Queue< kushekbaev::Queue< std::string > >& Q)
  {
    std::string string;
    while (std::getline(input, string))
    {
      if (string.empty())
      {
        continue;
      }
      kushekbaev::Queue< std::string > infixQ;
      splitExpr(string, infixQ);
      Q.push(infixQ);
    }
  }
}

void kushekbaev::openFile(const std::string& filename, Queue< Queue< std::string > >& Q)
{
  if (!filename.empty())
  {
    std::ifstream data(filename);
    {
      if (!data.is_open())
      {
        throw std::runtime_error("File couldn`t be opened!");
      }
      inputExpr(data, Q);
      data.close();
    }
  }
  else
  {
    inputExpr(std::cin, Q);
  }
}

void kushekbaev::output(std::ostream& output, Stack< long long int >& results)
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
