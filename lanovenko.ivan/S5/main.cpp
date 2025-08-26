#include <fstream>
#include <iostream>
#include <tree.hpp>
#include "accumulator.hpp"

int main(int argc, char * argv[])
{
  using namespace lanovenko;
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
  Tree< int, std::string, std::less< int > > biTree;
  while ((fileInput >> key >> value) && !fileInput.eof())
  {
    try
    {
      biTree.insert({key, value});
    }
    catch(...)
    {
      std::cerr << "Option error\n";
      return 1;
    }
  }
  if (!fileInput.eof())
  {
    std::cerr << "ERROR: wrong input\n";
    return 1;
  }
  try
  {
    std::string cmd(argv[1]);
    lanovenko::KeyValueAccumulator result;
    if (cmd == "ascending")
    {
      result = biTree.traverseLnr(result);
    }
    else if (cmd == "descending")
    {
      result = biTree.traverseRnl(result);
    }
    else if (cmd == "breadth")
    {
      result = biTree.traverseBreadth(result);
    }
    else
    {
      std::cerr << "<INVALID COMMAND>\n";
      return 1;
    }
    std::cout << result.keySum << ' ' << result.values << '\n';
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
