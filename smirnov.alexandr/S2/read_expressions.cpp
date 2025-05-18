#include "read_expressions.hpp"
#include <fstream>
#include <iostream>
#include "utils.hpp"

std::list< std::string > smirnov::readExpressions(const std::string & filename)
{
  std::list< std::string > expressions;
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

void smirnov::processInput(std::istream & in, std::list< std::string > & expressions) {
  std::string line;
  while (std::getline(in, line))
  {
    if (!line.empty())
    {
      expressions.push_back(line);
    }
  }
}
