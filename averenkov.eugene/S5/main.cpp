#include <iostream>
#include <fstream>
#include <BiTree.hpp>
#include "key_sum.hpp"
int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Error arguments\n";
    return 1;
  }

  std::string mode(argv[1]);
  std::string filename(argv[2]);

  averenkov::Tree< long long, std::string > bst;

  std::ifstream file(filename);
  if (!file)
  {
    std::cerr << "Error\n";
    return 1;
  }

  long long key;
  std::string value;
  while (file >> key >> value)
  {
    bst.push(key, value);
  }

  if (bst.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  try
  {
    averenkov::KeySum proc;
    if (mode == "ascending")
    {
      proc = bst.traverse_lnr(proc);
    }
    else if (mode == "descending")
    {
      proc = bst.traverse_rnl(proc);
    }
    else if (mode == "breadth")
    {
      proc = bst.traverse_breadth(proc);
    }
    else
    {
      std::cerr << "Error\n";
      return 1;
    }

    if (!proc.values_.empty())
    {
      proc.values_.pop_back();
    }
    std::cout << proc.sum_ << " " << proc.values_ << "\n";
  }
  catch (...)
  {
    std::cerr << "Error\n";
    return 1;
  }

  return 0;
}
