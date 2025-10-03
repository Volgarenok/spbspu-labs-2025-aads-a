#include <iostream>
#include <fstream>
#include <AVLtree.hpp>
#include "functionalObject.hpp"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Uncorrect input!\n";
    return 1;
  }
  std::fstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "File is not open!\n";
    return 1;
  }
  int key = 0;
  std::string value;
  finaev::AVLtree< int, std::string > tree;
  while (input >> key >> value)
  {
    tree.insert(std::make_pair(key, value));
  }
  if (!input.eof())
  {
    std::cerr << "Input error\n";
    return 1;
  }
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  std::string cmd = argv[1];
  finaev::keySum obj;
  try
  {
    if (cmd == "ascending")
    {
      obj = tree.traverse_lnr(obj);
    }
    else if (cmd == "descending")
    {
      obj = tree.traverse_rnl(obj);
    }
    else if (cmd == "breadth")
    {
      obj = tree.traverse_breadth(obj);
    }
    else
    {
      std::cerr << "<INVALID COMMAND>\n";
      return 1;
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  std::cout << obj.key_sum << " " << obj.values << "\n";
  return 0;
}
