#include "graphCollection.hpp"

bool smirnov::GraphCollection::addGraph(const std::string & name)
{
  auto it = graphs_.find(name);
  if (it != graphs_.end())
  {
    return false;
  }
  graphs_.insert(name, Graph(name));
  return true;
}

smirnov::Graph * smirnov::GraphCollection::getGraph(const std::string & name)
{
  auto it = graphs_.find(name);
  if (it != graphs_.end())
  {
    return std::addressof(const_cast< Graph & >(it->second));
  }
  return nullptr;
}

const smirnov::Graph * smirnov::GraphCollection::getGraph(const std::string & name) const
{
  auto it = graphs_.find(name);
  if (it != graphs_.cend())
  {
    return std::addressof(it->second);
  }
  return nullptr;
}

std::vector< std::string > smirnov::GraphCollection::listGraphs() const
{
  std::vector< std::string>  names;
  for (auto it = graphs_.cbegin(); it != graphs_.cend(); ++it)
  {
    names.push_back(it->first);
  }
  for (size_t i = 0; i < names.size(); ++i)
  {
    for (size_t j = i + 1; j < names.size(); ++j)
    {
      if (names[i] > names[j])
      {
        std::string temp = names[i];
        names[i] = names[j];
        names[j] = temp;
      }
    }
  }
  return names;
}
