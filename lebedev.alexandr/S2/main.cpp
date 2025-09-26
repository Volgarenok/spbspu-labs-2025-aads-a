#include <iostream>
#include <fstream>
#include "convertExpressions.hpp"

int main(int argc, char* argv[])
{
  
  if (argc > 2)
  {
    std::cerr << "Too much arguments!\n";
    return 1;
  }
  std::ifstream in;
  lebedev::Stack< std::string > res;
  try
  {
    if (argc == 2)
    {
      in.open(argv[1]);
      if (!in)
      {
        throw std::logic_error("Cannot open file!\n");
      }
    }
    if (in.is_open())
    {
      res = lebedev::inputExpressions(in);
    }
    else
    {
      res = lebedev::inputExpressions(std::cin);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (res.isEmpty())
  {
    std::cout << '\n';
    return 0;
  }
  else
  {
    std::cout << res.top();
    res.pop();
    while (!res.isEmpty())
    {
      std::cout << ' ' << res.top();
      res.pop();
    }
    std::cout << '\n';
  }
  return 0;
}
  
