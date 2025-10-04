#include "utils.hpp"

#include <array.hpp>

zholobov::Array< std::string > zholobov::splitTokens(const std::string& line)
{
  zholobov::Array< std::string > tokens;
  std::size_t pos = 0;
  while (true) {
    pos = line.find_first_not_of(" \t\r\n", pos);
    if (pos == std::string::npos) {
      break;
    }
    std::size_t end = line.find_first_of(" \t\r\n", pos);
    tokens.push_back(line.substr(pos, end - pos));
    if (end == std::string::npos) {
      break;
    }
    pos = end + 1;
  }
  return tokens;
}

std::istream& operator>>(std::istream& in, zholobov::Words& words)
{
  words.clear();
  std::string line;
  if (!std::getline(in, line)) {
    in.setstate(std::ios::eofbit);
    return in;
  }
  zholobov::Array< std::string > toks = zholobov::splitTokens(line);
  for (size_t i = 0; i < toks.size(); ++i) {
    words.push_back(toks[i]);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const zholobov::Words& words)
{
  if (!words.empty()) {
    out << words[0];
    for (size_t i = 1; i < words.size(); ++i) {
      out << " " << words[i];
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const zholobov::Dictionary& dict)
{
  for (const auto& entry: dict) {
    out << entry.first;
    if (!entry.second.empty()) {
      out << " " << entry.second;
    }
    out << '\n';
  }
  return out;
}
