#include <iostream>
#include <fstream>
#include <tree/tree-2-3.hpp>
#include "tree-sum-functor.hpp"

int main(int args, char** argv)
{
  using namespace alymova;
  if (args != 3)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  file.open(argv[2]);
  if (!file.is_open())
  {
    std::cerr << "<INCORRECT FILE>\n";
    return 1;
  }

  TwoThreeTree< int, std::string, std::less< int > > dataset;
  while (!file.eof() && !file.fail())
  {
    int key;
    std::string value;
    if (file >> key >> value && !value.empty())
    {
      dataset[key] = value;
    }
  }
  if (file.fail() && !file.eof())
  {
    std::cerr << "<INCORRECT MAP>\n";
    return 1;
  }

  if (dataset.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  try
  {
    std::string round = argv[1];
    TreeSum result;
    if (round == "ascending")
    {
      result = dataset.traverse_lnr(result);
    }
    else if (round == "descending")
    {
      result = dataset.traverse_rnl(result);
    }
    else if (round == "breadth")
    {
      result = dataset.traverse_breadth(result);
    }
    else
    {
      throw std::logic_error("<INCORRECT ARGUMENT>");
    }
    std::cout << result << '\n';
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
