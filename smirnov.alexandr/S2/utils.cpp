#include "utils.hpp"

bool smirnov::isOperator(const std::string & token)
{
  return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
}

bool smirnov::comparePriority(const std::string & op1, const std::string & op2)
{
  int p1 = 0;
  int p2 = 0;
  if (op1 == "+" || op1 == "-")
  {
    p1 = 1;
  }
  else if (op1 == "*" || op1 == "/" || op1 == "%")
  {
    p1 = 2;
  }
  if (op2 == "+" || op2 == "-")
  {
    p2 = 1;
  }
  else if (op2 == "*" || op2 == "/" || op2 == "%")
  {
    p2 = 2;
  }
  return p1 < p2;
}

smirnov::List< std::string > smirnov::splitBySpace(const std::string & line)
{
  List< std::string > tokens;
  size_t pos = 0;
  size_t next = 0;
  while ((next = line.find(' ', pos)) != std::string::npos)
  {
    tokens.push_front(line.substr(pos, next - pos));
    pos = next + 1;
  }
  if (pos < line.size())
  {
    tokens.push_front(line.substr(pos));
  }
  List< std::string > result;
  while (!tokens.empty())
  {
    result.push_front(tokens.front());
    tokens.pop_front();
  }
  return result;
}
