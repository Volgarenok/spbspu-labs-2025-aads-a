#include "ioExpression.hpp"
#include <fstream>
#include <iostream>

namespace
{
  void splitExpression(const std::string & str, maslov::Queue< std::string > & infixQueue)
  {
    std::string element;
    char separator = ' ';
    for (size_t i = 0; i < str.length(); ++i)
    {
      if (str[i] != separator)
      {
        element += str[i];
      }
      else
      {
        if (!element.empty())
        {
          infixQueue.push(element);
          element.clear();
        }
      }
    }
    if (!element.empty())
    {
      infixQueue.push(element);
    }
  }

  void inputExpression(std::istream & in, maslov::Queue< maslov::Queue< std::string > > & queue)
  {
    std::string str;
    while (std::getline(in, str))
    {
      if (str.empty())
      {
        continue;
      }
      maslov::Queue< std::string > infixQueue;
      splitExpression(str, infixQueue);
      queue.push(infixQueue);
    }
  }
}

void maslov::inputFile(const std::string & filename, Queue< Queue< std::string > > & queue)
{
  if (!filename.empty())
  {
    std::ifstream fileInput(filename);
    if (!fileInput.is_open())
    {
      throw std::runtime_error("ERROR: cannot open the file");
    }
    inputExpression(fileInput, queue);
    fileInput.close();
  }
  else
  {
    inputExpression(std::cin, queue);
  }
}

void maslov::printData(std::ostream & out, Stack< long long int > & results)
{
  if (!results.empty())
  {
    out << results.top();
    results.pop();
    while (!results.empty())
    {
      out << " " << results.top();
      results.pop();
    }
  }
}
