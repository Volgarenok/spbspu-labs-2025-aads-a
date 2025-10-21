#include <unordered_map>
#include "handle_functions.hpp"
namespace hismatova
{
  void handle_print(const std::unordered_map< std::string, BinarySearchTree< int, std::string > >& datasets, const std::string& datasetName)
  {
    auto it = datasets.find(datasetName);
    if (it == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    const auto& tree = it->second;
    if (tree.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }
    for (const auto& pair: tree)
    {
      const auto key = pair.first;
      const auto value = pair.second;
      std::cout << key << " " << value << "\n";
    }
  }
  void handle_complement(std::unordered_map< std::string, BinarySearchTree< int, std::string > >& datasets,
    const std::string& result,
    const std::string& a,
    const std::string& b)
  {
    if (datasets.find(a) == datasets.end() || datasets.find(b) == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    auto A = datasets.at(a);
    auto B = datasets.at(b);
    BinarySearchTree< int, std::string > res;
    for (const auto& [key, value]: A)
    {
      if (B.find(key) == B.end())
      {
        res.insert({key, value});
      }
    }
    datasets[result] = std::move(res);
  }
  void handle_intersect(std::unordered_map< std::string, BinarySearchTree< int, std::string > >& datasets,
    const std::string& result,
    const std::string& a,
    const std::string& b)
  {
    const auto itA = datasets.find(a);
    const auto itB = datasets.find(b);
    if (itA == datasets.end() || itB == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    auto A = datasets.at(a);
    auto B = datasets.at(b);
    BinarySearchTree< int, std::string > res;
    for (const auto& [key, value]: A)
    {
      if (B.find(key) != B.end())
      {
        res.insert({key, value});
      }
    }
    datasets[result] = std::move(res);
  }
  void handle_union(std::unordered_map< std::string, BinarySearchTree< int, std::string > >& datasets,
    const std::string& result,
    const std::string& a,
    const std::string& b)
  {
    const auto itA = datasets.find(a);
    const auto itB = datasets.find(b);
    if (itA == datasets.end() || itB == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    auto A = datasets.at(a);
    auto B = datasets.at(b);
    BinarySearchTree< int, std::string > res;
    for (const auto& [key, value]: A)
    {
      res.insert({key, value});
    }
    for (const auto& [key, value]: B)
    {
      res.insert({key, value});
    }
    datasets[result] = std::move(res);
  }
}
