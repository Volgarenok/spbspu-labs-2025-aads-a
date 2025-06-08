#include "utils.hpp"
#include <string>
#include <iostream>
#include <exception>

using namespace alymova;

void alymova::PrintCommand::operator()(const CompositeDataset_t& dicts)
{
  std::string name;
  in >> name;

  Dataset_t dataset = dicts.at(name);
  if (dataset.empty())
  {
    out << "<EMPTY>";
    return;
  }
  out << name;
  for (auto it = dataset.begin(); it != dataset.end(); it++)
  {
    out << ' ' << it->first << ' ' << it->second;
  }
}

void alymova::ComplementCommand::operator()(CompositeDataset_t& dicts)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;

  Dataset_t dataset1 = dicts.at(name1);
  Dataset_t dataset2 = dicts.at(name2);
  for (auto it = dataset1.begin(); it != dataset1.end(); it++)
  {
    if (dataset2.count(it->first) == 0)
    {
      dataset2.insert({it->first, it->second});
    }
    else
    {
      dataset2.erase(it->first);
    }
  }
  dicts[newname] = dataset2;
}

void alymova::IntersectCommand::operator()(CompositeDataset_t& dicts)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;

  Dataset_t dataset1 = dicts.at(name1);
  Dataset_t dataset2 = dicts.at(name2);
  for (auto it = dataset1.begin(); it != dataset1.end();)
  {
    if (dataset2.find(it->first) == dataset2.end())
    {
      it = dataset1.erase(it);
    }
    else
    {
      it++;
    }
  }
  dicts[newname] = dataset1;
}

void alymova::UnionCommand::operator()(CompositeDataset_t& dicts)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;

  Dataset_t dataset1 = dicts.at(name1);
  Dataset_t dataset2 = dicts.at(name2);
  for (auto it = dataset2.begin(); it != dataset2.end(); it++)
  {
    if (dataset1.count(it->first) == 0)
    {
      dataset1.insert({it->first, it->second});
    }
  }
  dicts[newname] = dataset1;
}

alymova::CompositeDataset_t alymova::readDictionaryFile(std::istream& in)
{
  CompositeDataset_t dataset_comp;
  std::string name;
  while (in >> name)
  {
    Dataset_t dataset;
    size_t key;
    std::string value;
    while (in.peek() != '\n' && in)
    {
      in >> key >> value;
      dataset[key] = value;
    }
    dataset_comp[name] = dataset;
  }
  if ((in).fail() && !(in).eof())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  return dataset_comp;
}

alymova::CommandDataset_t alymova::complectCommands()
{
  return
  {
    {"print", PrintCommand{std::cin, std::cout}},
    {"complement", ComplementCommand{std::cin}},
    {"intersect", IntersectCommand{std::cin}},
    {"union", UnionCommand{std::cin}}
  };
}
