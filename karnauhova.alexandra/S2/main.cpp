#include <iostream>
#include <fstream>
#include "stack.hpp"
#include "queue.hpp"
#include "input.hpp"

void output_sums(std::ostream& out, karnauhova::Stack< long long int > calc)
{
  if (!calc.empty())
  {
    long long int sum = calc.top();
    out << sum;
    calc.pop();
  }
  while (!calc.empty())
  {
    long long int sum = calc.top();
    out << " " << sum;
    calc.pop();
  }
  out << "\n";
}

int main(int argc, char** argv)
{
  karnauhova::Stack< long long int > calc;
  try
  {
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      if (file.is_open())
      {
        calc = karnauhova::inputStr(file);
        file.close();
      }
      else
      {
        std::cerr << "Incorrect name file\n";
        return 1;
      }
    }
    else
    {
      calc = karnauhova::inputStr(std::cin);
    }
    output_sums(std::cout, calc);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}



