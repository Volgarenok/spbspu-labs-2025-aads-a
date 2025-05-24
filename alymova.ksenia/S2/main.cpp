#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <stdexcept>
#include <queue.hpp>
#include <stack.hpp>
#include "postfix.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "Incorrect arguments\n";
    return 1;
  }
  std::istream* input = &std::cin;
  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "Incorrect file\n";
      return 1;
    }
    input = &file;
  }
  try
  {
    alymova::Stack< long long int > res;
    while (!(*input).eof())
    {
      std::string s;
      std::getline(*input, s);
      if (s.empty())
      {
        continue;
      }
      alymova::Postfix postfix(s);
      res.push(postfix());
    }
    if (!res.empty())
    {
      std::cout << res.top();
      res.pop();
    }
    while (!res.empty())
    {
      std::cout << " " << res.top();
      res.pop();
    }
    std::cout << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
