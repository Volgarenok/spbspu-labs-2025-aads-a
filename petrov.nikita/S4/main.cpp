#include <fstream>
#include <string>
#include <iostream>
#include <limits>
#include "avl_tree.hpp"
#include "commands.hpp"

namespace
{
  int checkArguments(const int argc);

  int checkArguments(const int argc)
  {
    if (argc == 2)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int main(int argc, const char * const * argv)
{
  using namespace petrov;
  if (!checkArguments(argc))
  {
    std::cerr << "ERROR: Incorrect number of arguments ";
    std::cerr << "\n";
    return 1;
  }
  using subtree_t = AVLTree< int, std::string, std::less< int > >;
  using maintree_t = AVLTree< std::string, subtree_t >;
  maintree_t tree;
  std::ifstream input(argv[1]);
  while (!input.eof())
  {
    subtree_t subtree;
    std::string dataset;
    input >> dataset;
    if (input.eof())
    {
      break;
    }
    else if (dataset.empty())
    {
      continue;
    }
    std::pair< int, std::string > data;
    while (input)
    {
      input >> data.first;
      input >> data.second;
      if (!input)
      {
        input.clear();
        break;
      }
      subtree.insert({ data.first, data.second });
    }
    tree.insert({ dataset, subtree });
  }
  std::string command_name;
  while (!std::cin.eof())
  {
    std::cin >> command_name;
    if (std::cin.eof())
    {
      continue;
    }
    try
    {
      if (command_name == "print")
      {
        print(std::cout, std::cin, tree);
      }
      else if (command_name == "complement")
      {
        complement(std::cin, tree);
      }
      else if (command_name == "intersect")
      {
        intersect(std::cin, tree);
      }
      else if (command_name == "union")
      {
        unionCMD(std::cin, tree);
      }
      else
      {
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cout << "<INVALID COMMAND>" << "\n";
      }
    }
    catch (const std::logic_error & e)
    {
      std::cout << e.what() << "\n";
    }
  }
}
