#include <iostream>
#include <fstream>
#include <string>
#include "calculatorExpression.hpp"

namespace
{
  void splitExpression(const std::string & str, maslov::Queue< std::string > & infixQueue)
  {
    char separator = ' ';
    size_t begin = str.find_first_not_of(separator);
    size_t end = 0;
    while (begin != std::string::npos)
    {
      end = str.find(separator, begin);
      std::string element = str.substr(begin, end - begin);
      infixQueue.push(element);
      begin = str.find_first_not_of(separator, end);
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

  void printData(std::ostream & out, maslov::Stack< long long int > & results)
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
}

int main(int argc, char ** argv)
{
  using namespace maslov;
  Queue< Queue< std::string > > queue;
  Stack< long long int > results;
  try
  {
    if (argc > 1)
    {
      std::ifstream fileInput(argv[1]);
      if (!fileInput.is_open())
      {
        std::cerr << "ERROR: cannot open the file\n";
        return 1;
      }
      inputExpression(fileInput, queue);
    }
    else
    {
      inputExpression(std::cin, queue);
    }
    while (!queue.empty())
    {
      Queue< std::string > infixQueue = queue.front();
      queue.pop();
      Queue< std::string > postfixQueue = infixToPostfix(infixQueue);
      long long int result = calculatePostfix(postfixQueue);
      results.push(result);
    }
    printData(std::cout, results);
    std::cout << "\n";
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
