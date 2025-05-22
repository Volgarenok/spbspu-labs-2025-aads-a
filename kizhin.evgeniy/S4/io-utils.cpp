#include "io-utils.hpp"
#include <iostream>

namespace kizhin {
  std::istream& operator>>(std::istream&, Dataset&);
  std::ostream& operator<<(std::ostream&, Dataset::const_reference);
  std::ostream& operator<<(std::ostream&, const Dataset&);
}

std::istream& kizhin::operator>>(std::istream& in, DSContainer& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DSContainer input;
  while (in) {
    DSContainer::key_type key;
    DSContainer::mapped_type val;
    in >> key >> val;
    if (!key.empty()) {
      input.emplace(std::move(key), std::move(val));
    }
  }
  if (!input.empty()) {
    in.clear();
    dest = input;
  }
  return in;
}

std::ostream& kizhin::operator<<(std::ostream& out, DSContainer::const_reference data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  out << data.first;
  if (!data.second.empty()) {
    out << ' ';
  }
  out << data.second;
  return out;
}

std::istream& kizhin::operator>>(std::istream& in, Dataset& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Dataset::key_type key{};
  Dataset::mapped_type val{};
  Dataset input;
  while (in >> key >> val) {
    input.emplace(std::move(key), std::move(val));
  }
  in.clear();
  dest = input;
  return in;
}

std::ostream& kizhin::operator<<(std::ostream& out, Dataset::const_reference data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  return out << data.first << ' ' << data.second;
}

std::ostream& kizhin::operator<<(std::ostream& out, const Dataset& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry || data.empty()) {
    return out;
  }
  auto it = data.begin();
  out << *(it++);
  for (auto end = data.end(); it != end; ++it) {
    out << ' ' << *it;
  }
  return out;
}

