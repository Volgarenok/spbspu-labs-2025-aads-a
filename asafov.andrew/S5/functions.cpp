#include "functions.h"
#include "lnr_iterator.hpp"
#include "rnl_iterator.hpp"
#include "nlr_iterator.hpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <limits>

int parseFile(const std::string& filename, asafov::map< int, std::string >& tree)
{
  std::ifstream file(filename);
  if (!file)
  {
    std::cerr << "Error: Cannot open file '" << filename << "'\n";
    return 1;
  }

  int key;
  std::string value;
  while (file >> key >> value)
  {
    tree.insert({key, value});
  }

  return 0;
}

bool check_overflow(long long sum, int add)
{
  return (add > 0 && sum > INT_MAX - add) || (add < 0 && sum < INT_MIN - add);
}

int traverseTree(const std::string& mode, const asafov::map< int, std::string >& tree)
{
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  long long sum = 0;

  if (mode == "ascending")
  {
    for (asafov::lnr_iterator< int, std::string > it(tree.begin().operator->()), end; it != end; ++it)
    {
      int k = it->first;
      if (check_overflow(sum, k))
      {
        std::cerr << "Error: Integer overflow.\n";
        return 1;
      }
      sum += k;
      std::cout << ' ' << it->second;
    }
  }
  else if (mode == "descending")
  {
    for (asafov::rnl_iterator< int, std::string > it(tree.begin().operator->()), end; it != end; ++it)
    {
      int k = it->first;
      if (check_overflow(sum, k))
      {
        std::cerr << "Error: Integer overflow.\n";
        return 1;
      }
      sum += k;
      std::cout << ' ' << it->second;
    }
  }
  else if (mode == "breadth")
  {
    std::queue< decltype(tree.begin().operator->()) > q;
    auto* root = tree.begin().operator->();
    q.push(root);

    while (!q.empty())
    {
      auto* node = q.front();
      q.pop();

      int k = node->data.first;
      if (check_overflow(sum, k))
      {
        std::cerr << "Error: Integer overflow.\n";
        return 1;
      }
      sum += k;
      std::cout << ' ' << node->data.second;

      if (node->left) q.push(node->left);
      if (node->right) q.push(node->right);
    }
  }
  else
  {
    std::cerr << "Error: Unknown traversal mode '" << mode << "'\n";
    return 1;
  }

  std::cout << "\n" << sum << "\n";
  return 0;
}
