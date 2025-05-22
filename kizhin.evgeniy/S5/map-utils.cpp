#include "map-utils.hpp"
#include <istream>
#include <limits>

std::istream& kizhin::operator>>(std::istream& in, MapT& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  MapT::key_type key{};
  MapT::mapped_type val{};
  MapT input;
  while (in >> key >> val) {
    input.emplace(std::move(key), std::move(val));
  }
  in.clear();
  dest = input;
  return in;
  return in;
}

void kizhin::ValueCollector::operator()(MapT::const_reference value)
{
  const MapT::key_type& key = value.first;
  if (key > 0 && keys > std::numeric_limits< MapT::key_type >::max() - key) {
    throw std::overflow_error("Overflow");
  }
  if (key < 0 && keys < std::numeric_limits< MapT::key_type >::min() - key) {
    throw std::underflow_error("Underflow");
  }
  keys += value.first;
  if (!values.empty()) {
    values += ' ';
  }
  values += value.second;
}

