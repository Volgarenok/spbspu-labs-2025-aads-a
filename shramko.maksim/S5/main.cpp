#include <iostream>
#include <fstream>
#include <UBST.hpp>

#include "key_sum.hpp"

int main(int argc, char* argv[])
{
  using namespace shramko;

  if (argc != 3)
  {
    std::cerr << "Invalid arguments" << std::endl;
    return 1;
  }

  std::ifstream file(argv[2]);
  if (!file.is_open())
  {
    std::cerr << "File open failed" << std::endl;
    return 1;
  }

  UBstTree<int, std::string> dict;
  int key = 0;
  std::string value = "";
  while (file >> key >> value)
  {
    dict[key] = value;
  }

  if (dict.empty())
  {
    std::cout << "<EMPTY>" << std::endl;
    return 0;
  }

  std::string command = argv[1];
  KeySum func;
  try
  {
    if (command == "ascending")
    {
      func = dict.traverse_lnr(func);
    }
    else if (command == "descending")
    {
      func = dict.traverse_rnl(func);
    }
    else if (command == "breadth")
    {
      func = dict.traverse_breadth(func);
    }
    else
    {
      std::cerr << "Invalid command" << std::endl;
      return 1;
    }
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  std::cout << func.result;
  if (!func.elems.empty())
  {
    std::cout << " " << func.elems;
  }
  std::cout << std::endl;
  return 0;
}
