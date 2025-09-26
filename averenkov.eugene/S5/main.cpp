#include <map>
#include <iostream>
#include <fstream>
#include <BiTree.hpp>
#include "key_sum.hpp"

int main(int argc, char* argv[])
{
  using MyTree = averenkov::Tree< long long, std::string >;

  if (argc != 3)
  {
    std::cerr << "Error arguments\n";
    return 1;
  }

  std::string mode(argv[1]);
  std::string filename(argv[2]);

  MyTree bst;

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

  using TraverseMethod = averenkov::KeySum (MyTree::*)(averenkov::KeySum);

  averenkov::Tree< std::string, TraverseMethod > methodMap;
  methodMap["ascending"] = &MyTree::traverse_lnr;
  methodMap["descending"] = &MyTree::traverse_rnl;
  methodMap["breadth"] = &MyTree::traverse_breadth;

  try
  {
    averenkov::KeySum proc;
    auto method = methodMap.at(mode);
    proc = (bst.*method)(proc);
    if (!proc.values_.empty())
    {
      proc.values_.pop_back();
    }
    std::cout << proc.sum_ << " " << proc.values_ << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
