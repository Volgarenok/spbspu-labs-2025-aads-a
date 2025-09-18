#include "command-processor.hpp"
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include "io-utils.hpp"

namespace kizhin {
  void print(const DSContainer&, std::istream&, std::ostream&);
  void complement(DSContainer&, std::istream&);
  void intersect(DSContainer&, std::istream&);
  void unionCmd(DSContainer&, std::istream&);
}

void kizhin::processCommands(DSContainer& datasets, std::istream& in, std::ostream& out)
{
  using CmdContainer = std::map< std::string, std::function< void(void) > >;
  const auto inRef = std::ref(in);
  const auto outRef = std::ref(out);
  const CmdContainer commands = {
    { "print", std::bind(print, std::cref(datasets), inRef, outRef) },
    { "complement", std::bind(complement, std::ref(datasets), inRef) },
    { "intersect", std::bind(intersect, std::ref(datasets), inRef) },
    { "union", std::bind(unionCmd, std::ref(datasets), inRef) },
  };
  CmdContainer::key_type currKey;
  while (in >> currKey) {
    try {
      commands.at(currKey)();
    } catch (const std::logic_error&) {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

void kizhin::print(const DSContainer& datasets, std::istream& in, std::ostream& out)
{
  DSContainer::key_type key;
  if (!(in >> key)) {
    throw std::logic_error("Invalid print command");
  }
  auto it = datasets.find(key);
  if (it == datasets.end()) {
    throw std::logic_error("Failed to find key: " + key);
  }
  if (it->second.empty()) {
    out << "<EMPTY>\n";
  } else {
    out << *it << '\n';
  }
}

void kizhin::complement(DSContainer& datasets, std::istream& in)
{
  DSContainer::key_type newName;
  DSContainer::key_type name1;
  DSContainer::key_type name2;
  if (!(in >> newName >> name1 >> name2)) {
    throw std::logic_error("Invalid complement command");
  }
  const auto& ds1 = datasets.at(name1);
  const auto& ds2 = datasets.at(name2);
  Dataset result;
  for (const auto& p: ds1) {
    if (!ds2.count(p.first)) {
      result[p.first] = p.second;
    }
  }
  datasets[newName] = result;
}

void kizhin::intersect(DSContainer& datasets, std::istream& in)
{
  DSContainer::key_type newName;
  DSContainer::key_type name1;
  DSContainer::key_type name2;
  if (!(in >> newName >> name1 >> name2)) {
    throw std::logic_error("Invalid intersect command");
  }
  const auto& ds1 = datasets.at(name1);
  const auto& ds2 = datasets.at(name2);
  Dataset result;
  for (const auto& p: ds1) {
    if (ds2.count(p.first)) {
      result[p.first] = p.second;
    }
  }
  datasets[newName] = result;
}

void kizhin::unionCmd(DSContainer& datasets, std::istream& in)
{

  DSContainer::key_type newName;
  DSContainer::key_type name1;
  DSContainer::key_type name2;
  if (!(in >> newName >> name1 >> name2)) {
    throw std::logic_error("Invalid union command");
  }
  const auto& ds1 = datasets.at(name1);
  const auto& ds2 = datasets.at(name2);
  Dataset result = ds1;
  for (const auto& p: ds2) {
    if (!result.count(p.first)) {
      result[p.first] = p.second;
    }
  }
  datasets[newName] = result;
}

