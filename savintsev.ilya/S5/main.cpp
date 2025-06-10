#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <two-three-tree.h>

namespace savintsev
{
  struct KeyAdder
  {
    void operator()(const std::pair< long long, std::string > val)
    {
      if (val.first > 0 && result_ > std::numeric_limits< long long >::max() - val.first)
      {
        throw std::overflow_error("Key sum overflow (positive)");
      }
      else if (val.first < 0 && result_ < std::numeric_limits< long long >::min() - val.first)
      {
        throw std::overflow_error("Key sum overflow (negative)");
      }

      result_ += val.first;
      values_ += " " + val.second;
    }

    long long result_ = 0;
    std::string values_;
  };
}

int main(int argc, char * argv[])
{
  if (argc != 3)
  {
    std::cerr << "ERROR: Invalid number of arguments\n";
    return 1;
  }

  std::string treverse_mode(argv[1]);
  std::string filename(argv[2]);

  using namespace savintsev;

  TwoThreeTree< long long, std::string > tree;

  std::ifstream file(filename);
  if (!file)
  {
    std::cerr << "ERROR: Can't open file\n";
    return 1;
  }

  long long key;
  std::string value;
  while (file >> key >> value)
  {
    tree.insert({key, value});
  }

  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  try
  {
    KeyAdder summator;

    if (treverse_mode == "ascending")
    {
      summator = tree.traverse_lnr(summator);
    }
    else if (treverse_mode == "descending")
    {
      summator = tree.traverse_rnl(summator);
    }
    else if (treverse_mode == "breadth")
    {
      summator = tree.traverse_breadth(summator);
    }
    else
    {
      std::cerr << "ERROR: Traverse doesn't exist (Invalid argument)\n";
      return 1;
    }

    std::cout << summator.result_ << summator.values_ << "\n";
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << "ERROR: " << e.what() << "\n";
    return 1;
  }
  catch (const std::logic_error & e)
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
}
