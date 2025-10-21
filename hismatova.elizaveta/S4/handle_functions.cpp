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
    const auto itA = datasets.find(a);
    const auto itB = datasets.find(b);
    if (itA == datasets.end() || itB == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    BinarySearchTree< int, std::string > res;
    for (const auto& pair: itA->second)
    {
      const auto key = pair.first;
      const auto value = pair.second;
      if (itB->second.find(key) == itB->second.end())
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
    BinarySearchTree< int, std::string > res;
    for (const auto& pair: itA->second)
    {
      const auto key = pair.first;
      const auto value = pair.second;
      if (itB->second.find(key) != itB->second.end())
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
    BinarySearchTree< int, std::string > res;
    for (const auto& pair: itB->second)
    {
      const auto key = pair.first;
      const auto value = pair.second;
      res.insert({key, value});
    }
    for (const auto& pair: itA->second)
    {
      const auto key = pair.first;
      const auto value = pair.second;
      res.insert({key, value});
    }
    datasets[result] = std::move(res);
  }
}
