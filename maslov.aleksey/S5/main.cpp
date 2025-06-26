#include <fstream>
#include <iostream>
#include <tree.hpp>
#include <functional>
#include "accumulator.hpp"

namespace
{
  using Tree = maslov::BiTree< int, std::string, std::less< int > >;
  void callAscending(const Tree & tree, maslov::KeyValueAccumulator & result)
  {
    result = tree.traverseLnr(result);
  }

  void callDescending(const Tree & tree, maslov::KeyValueAccumulator & result)
  {
    result = tree.traverseRnl(result);
  }

  void callBreadth(const Tree & tree, maslov::KeyValueAccumulator & result)
  {
    result = tree.traverseBreadth(result);
  }
}

int main(int argc, char * argv[])
{
  using namespace maslov;
  if (argc != 3)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }
  std::ifstream fileInput(argv[2]);
  if (!fileInput.is_open())
  {
    std::cerr << "ERROR: cannot open the file\n";
    return 1;
  }
  int key = 0;
  std::string value;
  BiTree< int, std::string, std::less< int > > biTree;
  while ((fileInput >> key >> value) && !fileInput.eof())
  {
    biTree.push(key, value);
  }
  if (!fileInput.eof())
  {
    std::cerr << "ERROR: wrong input\n";
    return 1;
  }
  using func = std::function< void(KeyValueAccumulator &) >;
  BiTree< std::string, func, std::less< std::string > > cmds;
  using namespace std::placeholders;
  cmds["ascending"] = std::bind(callAscending, std::cref(biTree), _1);
  cmds["descending"] = std::bind(callDescending, std::cref(biTree), _1);
  cmds["breadth"] = std::bind(callBreadth, std::cref(biTree), _1);
  try
  {
    KeyValueAccumulator result;
    cmds.at(argv[1])(result);
    std::cout << result.keySum << ' ' << result.values << '\n';
  }
  catch (const std::out_of_range & e)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
  catch (const std::logic_error & e)
  {
    std::cout << e.what() << '\n';
    return 0;
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
