#include <iostream>
#include <fstream>
#include <tree/avlTee.hpp>
#include "key_sum.hpp"

int main(int argc, char* argv[])
{
  using namespace smirnov;
  AvlTree< int, std::string > dict;
  if (argc == 3)
  {
    int key = 0;
    std::string value = "";
    std::ifstream file(argv[2]);
    while (file >> key >> value)
    {
      dict[key] = value;
    }
    if (!file.eof())
    {
      std::cerr << "Bad input file\n";
      return 1;
    }
  }
  else
  {
    std::cerr << "No such file\n";
    return 1;
  }

  if (dict.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  try
  {
    KeySum func;
    std::string mode(argv[1]);
    if (mode == "ascending")
    {
      func = dict.traverse_lnr(func);
    }
    else if (mode == "descending")
    {
      func = dict.traverse_rnl(func);
    }
    else if (mode == "breadth")
    {
      func = dict.traverse_breadth(func);
    }
    else
    {
      std::cerr << "Invalid traversal mode\n";
      return 1;
    }
    std::cout << func.result;
    if (!func.elems.empty())
    {
      std::cout << " " << func.elems;
    }
    std::cout << "\n";
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
