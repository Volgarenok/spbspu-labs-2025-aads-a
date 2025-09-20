#include <limits>
#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <avl_tree.hpp>
#include "commands.hpp"

namespace
{
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

namespace petrov
{
  using subtree_t = AVLTree< int, std::string, std::less< int > >;
  using maintree_t = AVLTree< std::string, subtree_t >;
  std::istream & inputDatasets(std::istream & in, maintree_t & tree);
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

  maintree_t tree;
  std::ifstream input(argv[1]);
  try
  {
    inputDatasets(input, tree);
  }
  catch (const std::bad_alloc & e)
  {
    std::cerr << e.what() << "\n";
    return 2;
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 3;
  }

  AVLTree< std::string, std::function< void() > > cmds;
  cmds.insert({ "print", std::bind(print, std::ref(std::cout), std::ref(std::cin), std::cref(tree))});
  cmds.insert({ "complement", std::bind(complement, std::ref(std::cin), std::ref(tree))});
  cmds.insert({ "intersect", std::bind(intersect, std::ref(std::cin), std::ref(tree))});
  cmds.insert({ "union", std::bind(unionCMD, std::ref(std::cin), std::ref(tree))});

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::logic_error & e)
    {
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << e.what() << "\n";
    }
    catch (const std::bad_alloc & e)
    {
      std::cerr << e.what() << "\n";
      return 2;
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << "\n";
      return 3;
    }
  }
}

std::istream & petrov::inputDatasets(std::istream & input, maintree_t & tree)
{
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
  return input;
}
