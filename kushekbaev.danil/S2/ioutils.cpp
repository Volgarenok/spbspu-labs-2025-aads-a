#include "ioutils.hpp"
#include <iostream>

void kushekbaev::splitExpr(const std::string& string, Queue< std::string >& arithmeticQueue)
{
  std::string symbol;
  char delimiter = ' ';
  size_t begin = string.find_first_not_of(delimiter);
  size_t end = 0;
  while (begin != std::string::npos)
  {
    end = string.find(delimiter, begin);
    symbol = string.substr(begin, end - begin);
    arithmeticQueue.push(symbol);
    begin = string.find_first_not_of(delimiter, end);
  }
}

void kushekbaev::inputExpr(std::istream& input, Queue< Queue< std::string > >& queue)
{
  std::string string;
  kushekbaev::Queue< std::string > infixQueue;
  while (std::getline(input, string))
  {
    if (string.empty())
    {
      continue;
    }
    infixQueue.clear();
    splitExpr(string, infixQueue);
    queue.push(std::move(infixQueue));
  }
}

void kushekbaev::output(std::ostream& output, const Stack< long long int >& results)
{
  auto out_results = results;
  if (!out_results.empty())
  {
    output << out_results.top();
    out_results.pop();
    while (!out_results.empty())
    {
      output << " " << out_results.top();
      out_results.pop();
    }
  }
}
