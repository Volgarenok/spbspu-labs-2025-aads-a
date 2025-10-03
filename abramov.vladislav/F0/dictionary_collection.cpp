#include "dictionary_collection.hpp"
#include <stdexcept>

abramov::DictionaryCollection::DictionaryCollection():
  collection_(HashTable< std::string, Dictionary >{})
{}

abramov::Dictionary &abramov::DictionaryCollection::findDict(const std::string &name)
{
  return const_cast< Dictionary& >(static_cast< const DictionaryCollection* >(this)->cfindDict(name));
}

const abramov::Dictionary &abramov::DictionaryCollection::cfindDict(const std::string &name) const
{
  auto it = collection_.cfind(name);
  if (it == collection_.cend())
  {
    throw std::logic_error("There is no such dictionary\n");
  }
  return it->second;
}

void abramov::DictionaryCollection::addDict(const std::string &name)
{
  if (collection_.find(name) == collection_.end())
  {
    collection_.insert(name, Dictionary());
  }
  else
  {
    throw std::logic_error("There is dictionary with such name\n");
  }
}

void abramov::DictionaryCollection::addCompleteDict(const std::string &name, const Dictionary &dict)
{
  if (collection_.find(name) == collection_.end())
  {
    collection_.insert(name, dict);
  }
  else
  {
    throw std::logic_error("There is dictionary with such name\n");
  }
}

void abramov::DictionaryCollection::deleteDict(const std::string &name)
{
  if (!collection_.erase(name))
  {
    throw std::logic_error("There is no such dictionary\n");
  }
}
