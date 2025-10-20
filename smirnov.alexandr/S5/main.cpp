#include <iostream>
#include <fstream>
#include <functional>
#include <tree/avlTee.hpp>
#include "key_sum.hpp"

namespace
{
  using Tree = smirnov::AvlTree< int, std::string >;
  using Result = smirnov::KeySum;
  void traverseLnr(const Tree & tree, Result & res)
  {
    res = tree.traverse_lnr(res);
  }
  void traverseRnl(const Tree & tree, Result & res)
  {
    res = tree.traverse_rnl(res);
  }
  void traverseBreadth(const Tree & tree, Result & res)
  {
    res = tree.traverse_breadth(res);
  }
}

int main(int argc, char * argv[])
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
  AvlTree< std::string, std::function< void(KeySum &) > > commands;
  using namespace std::placeholders;
  commands["ascending"] = std::bind(traverseLnr, std::cref(dict), _1);
  commands["descending"] = std::bind(traverseRnl, std::cref(dict), _1);
  commands["breadth"] = std::bind(traverseBreadth, std::cref(dict), _1);
  std::string command = argv[1];
  KeySum func;
  try
  {
    commands.at(command)(func);
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
  std::cout << func.result;
  if (!func.elems.empty())
  {
    std::cout << " " << func.elems;
  }
  std::cout << "\n";
}
