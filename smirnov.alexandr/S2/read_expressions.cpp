#include "read_expressions.hpp"
#include <fstream>
#include <iostream>
#include <list/list_utils.hpp>
#include "utils.hpp"

smirnov::List< std::string > smirnov::readExpressions(const std::string & filename)
{
  smirnov::List< std::string > expressions;
  if (filename.empty())
  {
    processInput(std::cin, expressions);
  }
  else
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      return expressions;
    }
    processInput(file, expressions);
  }
  return expressions;
}

void smirnov::processInput(std::istream & in, smirnov::List< std::string > & expressions)
{
  std::string line;
  while (std::getline(in, line))
  {
    if (!line.empty())
    {
      pushBack(expressions, line);
    }
  }
}
