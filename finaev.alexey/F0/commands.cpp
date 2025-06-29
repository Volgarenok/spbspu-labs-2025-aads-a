#include "commands.hpp"
#include <limits>
#include <ostream>
#include <functional>
#include <random>
#include <fstream>
#include <iterator>
#include <dynamicArr.hpp>

namespace
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char > &s);
    ~StreamGuard();
    StreamGuard(const StreamGuard&) = delete;
    StreamGuard& operator=(const StreamGuard&) = delete;
    StreamGuard(StreamGuard&&) = delete;
    StreamGuard& operator=(StreamGuard&&) = delete;
  private:
    std::basic_ios< char > &s_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };

  StreamGuard::StreamGuard(std::basic_ios< char > &s):
    s_(s),
    precision_(s.precision()),
    width_(s.width()),
    flags_(s.flags()),
    fill_(s.fill())
  {}

  StreamGuard::~StreamGuard()
  {
    s_.precision(precision_);
    s_.width(width_);
    s_.flags(flags_);
    s_.fill(fill_);
  }

  bool operator==(const finaev::DynamicArr< std::string >& lhs, const finaev::DynamicArr< std::string >& rhs)
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    for (size_t i = 0; i < lhs.size(); ++i)
    {
      if (lhs[i] != rhs[i])
      {
        return false;
      }
    }
    return true;
  }

  void processLineString(const std::string& line, finaev::HashTable< std::string, finaev::OpenningInfo >& debuts, std::ostream& out)
  {
    size_t pos = 0;
    size_t space_pos = line.find(' ', pos);
    if (space_pos == std::string::npos)
    {
      return;
    }
    std::string key = line.substr(pos, space_pos - pos);
    pos = space_pos + 1;
    if (debuts.find(key) != debuts.end())
    {
      return;
    }
    finaev::OpenningInfo inf;
    pos = line.find('"', pos);
    if (pos == std::string::npos)
    {
      return;
    }
    size_t end_moves = line.find('"', pos + 1);
    if (end_moves == std::string::npos)
    {
      return;
    }
    std::string moves_str = line.substr(pos + 1, end_moves - pos - 1);
    finaev::DebutMoves moves;
    size_t move_start = 0;
    size_t move_end = moves_str.find(' ');
    while (move_end != std::string::npos)
    {
      std::string move = moves_str.substr(move_start, move_end - move_start);
      if (!move.empty())
      {
        moves.moves.push(move);
      }
      move_start = move_end + 1;
      move_end = moves_str.find(' ', move_start);
    }
    std::string last_move = moves_str.substr(move_start);
    if (!last_move.empty())
    {
      moves.moves.push(last_move);
    }
    inf.moves = moves;
    pos = end_moves + 1;
    pos = line.find('"', pos);
    if (pos == std::string::npos)
    {
      return;
    }
    end_moves = line.find('"', pos + 1);
    if (end_moves == std::string::npos)
    {
      return;
    }
    inf.name = line.substr(pos + 1, end_moves - pos - 1);
    pos = end_moves + 1;
    pos = line.find('"', pos);
    if (pos == std::string::npos)
    {
      return;
    }
    end_moves = line.find('"', pos + 1);
    if (end_moves == std::string::npos)
    {
      return;
    }
    inf.description = line.substr(pos + 1, end_moves - pos - 1);
    debuts[key] = inf;
    out << "Opening " << key << " successfully added\n";
  }

  bool isExactMoves(const finaev::DebutMoves& f, const finaev::DebutMoves& s)
  {
    return f.moves == s.moves;
  }

  using pair = std::pair< std::string, bool >;
  using arr = finaev::DynamicArr< std::pair< std::string, std::string > >;
  using hashTable = finaev::HashTable< std::string, bool >;
  void collectResults(const finaev::globalDebuts& debuts, arr& results, const finaev::DebutMoves& mov, const pair& baseEntry)
  {
    auto debut = debuts.find(baseEntry.first);
    if (debut != debuts.cend() && debut->second.moves.containsSequence(mov.moves))
    {
      results.push(std::make_pair(baseEntry.first, debut->second.name));
    }
  }

  bool compareKeys(const std::pair< std::string, std::string >& a, const std::pair< std::string, std::string >& b)
  {
    return a.first < b.first;
  }

  void formatOutput(std::ostream& out, const std::pair< std::string, std::string >& pair, bool& isFirst)
  {
    if (!isFirst)
    {
      out << "\n";
    }
    out << "- " << pair.first << ": " << pair.second;
    isFirst = false;
  }

  void printOpening(std::ostream& out, const finaev::globalDebuts& debuts, const pair& base, bool& isFirst)
  {
    auto it = debuts.find(base.first);
    if (it != debuts.cend())
    {
      if (!isFirst)
      {
        out << "\n";
      }
      out << "- " << it->first << ": " << it->second.name;
      isFirst = false;
    }
  }

  size_t mergeEntries(hashTable& target, const hashTable& source, const finaev::globalDebuts& debuts)
  {
    size_t added = 0;
    for (auto it = source.cbegin(); it != source.cend(); ++it)
    {
      if (debuts.find(it->first) == debuts.cend())
      {
        continue;
      }
      auto result = target.insert(*it);
      if (result.second)
      {
        ++added;
      }
    }
    return added;
  }

  void findIntersection(const hashTable& base1, const hashTable& base2, const finaev::globalDebuts& debuts, hashTable& result)
  {
    for (auto it = base2.cbegin(); it != base2.cend(); ++it)
    {
      const std::string& key = it->first;
      if (base1.find(key) != base1.cend() && debuts.find(key) != debuts.cend())
      {
        result.insert(*it);
      }
    }
  }

  void findComplement(const hashTable& base1, const hashTable& base2, const finaev::globalDebuts& debuts, hashTable& result)
  {
    for (auto it = base2.cbegin(); it != base2.cend(); ++it)
    {
      const std::string& key = it->first;
      if (base1.find(key) == base1.cend() && debuts.find(key) != debuts.cend())
      {
        result.insert(*it);
      }
    }
  }

  bool isValidChessSquare(const std::string& sq)
  {
    if (sq.length() != 2)
    {
      return false;
    }
    char file = tolower(sq[0]);
    char rank = sq[1];
    return (file >= 'a' && file <= 'h') && (rank >= '1' && rank <= '8');
  }

  bool isValidChessMove(const std::string& move)
  {
    if (move.length() != 5 || move[2] != '-')
    {
      return false;
    }
    std::string from = move.substr(0, 2);
    std::string to = move.substr(3, 2);
    return isValidChessSquare(from) && isValidChessSquare(to);
  }

  bool areAllMovesValid(const finaev::DebutMoves& moves)
  {
    for (size_t i = 0; i < moves.moves.size(); ++i)
    {
      const std::string& move = moves.moves[i];
      if (!isValidChessMove(move))
      {
        return false;
      }
    }
    return true;
  }
}

void finaev::loadDebutsFromFile(const std::string& filename, globalDebuts& debuts, std::ostream& out)
{
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("Error opening file");
  }
  std::string line;
  while (std::getline(file, line))
  {
    processLineString(line, debuts, out);
  }
}

void finaev::create_debut(std::istream& in, std::ostream& out, globalDebuts& debuts)
{
  std::string key;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) != debuts.end())
  {
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    throw std::runtime_error("<DUPLICATE>");
  }
  StreamGuard guard(in);
  OpenningInfo inf;
  if (!(in >> inf.moves))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  in >> std::ws;
  char quote;
  in >> quote;
  if (quote != '"')
  {
    in.setstate(std::ios::failbit);
  }
  std::getline(in, inf.name, '"');
  in >> std::ws;
  in >> quote;
  if (quote != '"')
  {
    in.setstate(std::ios::failbit);
  }
  std::getline(in, inf.description, '"');
  debuts[key] = inf;
  out << "Openning " << key << " successfully added";
}

void finaev::create_base(std::istream& in, std::ostream& out, debutsBases& db)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (db.find(baseName) != db.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  db[baseName] = HashTable< std::string, bool >();
  out << "Base " << baseName << " successfully added";
}

void finaev::add(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string baseName, debut;
  if (!(in >> baseName >> debut))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(debut) == debuts.cend())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  if (bases.find(baseName) == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto& base = bases[baseName];
  if (base.find(debut) != base.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  base[debut] = true;
  out << "Debut " << debut <<  " successfully added in base " << baseName;
}

void finaev::exact_find(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  DebutMoves mov;
  in >> mov;
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(baseName) == bases.cend())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  const auto& base = bases.at(baseName);
  bool found = false;
  std::string foundKey;
  for (auto it = base.cbegin(); it != base.cend(); ++it)
  {
    auto debutIt = debuts.find(it->first);
    if (debutIt != debuts.cend() && isExactMoves(debutIt->second.moves, mov))
    {
      found = true;
      foundKey = it->first;
      break;
    }
  }
  if (found)
  {
    const auto& opening = debuts.at(foundKey);
    out << "Key: " << foundKey << "\n";
    out << "Name: " << opening.name;
  }
  else
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
}

void finaev::find(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  DebutMoves mov;
  in >> mov;
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(baseName) == bases.cend())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto base = bases.find(baseName);
  DynamicArr< std::pair< std::string, std::string > > foundOpenings;
  const auto& baseEntries = base->second;
  for (auto entryIt = baseEntries.cbegin(); entryIt != baseEntries.cend(); ++entryIt)
  {
    collectResults(debuts, foundOpenings, mov, *entryIt);
  }
  for (size_t i = 0; i < foundOpenings.size(); ++i)
  {
    for (size_t j = i + 1; j < foundOpenings.size(); ++j)
    {
      if (!compareKeys(foundOpenings[i], foundOpenings[j]))
      {
        std::swap(foundOpenings[i], foundOpenings[j]);
      }
    }
  }
  if (foundOpenings.empty())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  out << "Found " << foundOpenings.size() << " openings:\n";
  bool isFirst = true;
  for (size_t i = 0; i < foundOpenings.size(); ++i)
  {
    formatOutput(out, foundOpenings[i], isFirst);
  }
}

void finaev::print(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto base = bases.find(baseName);
  if (base == bases.cend())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  bool isFirst = true;
  for (auto i = base->second.cbegin(); i != base->second.cend(); ++i)
  {
    printOpening(out, debuts, *i, isFirst);
  }
}

void finaev::move(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string sourceBase, targetBase, key, temp;
  if (!(in >> sourceBase >> targetBase >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto srcBase = bases.find(sourceBase);
  if (srcBase == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto trgBase = bases.find(targetBase);
  if (trgBase == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto debut = srcBase->second.find(key);
  if (debut == srcBase->second.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  if (debuts.find(key) == debuts.cend())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  if (trgBase->second.find(key) != trgBase->second.end())
  {
    out << "Debut " << key << " exists in target base";
    return;
  }
  trgBase->second[key] = true;
  srcBase->second.erase(debut);
  out << "Debut " << key << " moved from " << sourceBase << " to " << targetBase;
}

void finaev::merge(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string targetBase, sourceBase, temp;
  if (!(in >> targetBase >> sourceBase))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto srcBase = bases.find(sourceBase);
  if (srcBase == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto trgBase = bases.find(targetBase);
  if (trgBase == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  size_t addedCount = mergeEntries(trgBase->second, trgBase->second, debuts);
  out << "Merged " << sourceBase << " into " << targetBase << ": "<< addedCount << " debuts added";
}

void finaev::intersect(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string newBaseName, base1Name, base2Name, temp;
  if (!(in >> newBaseName >> base1Name >> base2Name))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(newBaseName) != bases.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  auto base1 = bases.find(base1Name);
  if (base1 == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto base2 = bases.find(base2Name);
  if (base2 == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  finaev::HashTable< std::string, bool > intersectionBase;
  findIntersection(base1->second, base2->second, debuts, intersectionBase);
  bases[newBaseName] = std::move(intersectionBase);
  out << "Created base " << newBaseName << " with " << bases[newBaseName].size();
  out << " openings (intersection of " << base1Name << " and " << base2Name << ")";
}

void finaev::complement(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string newBaseName, base1Name, base2Name, temp;
  if (!(in >> newBaseName >> base1Name >> base2Name))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(newBaseName) != bases.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  auto base1 = bases.find(base1Name);
  if (base1 == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto base2 = bases.find(base2Name);
  if (base2 == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  finaev::HashTable< std::string, bool > complementionBase;
  findComplement(base1->second, base2->second, debuts, complementionBase);
  bases[newBaseName] = std::move(complementionBase);
  out << "Created base " << newBaseName << " with " << bases[newBaseName].size();
  out << " openings (complemention of " << base2Name << " from " << base1Name << ")";
}

void finaev::remove(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string baseName, key, temp;
  if (!(in >> baseName >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) == debuts.cend())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  auto base = bases.find(baseName);
  if (base == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto debut = base->second.find(key);
  if (debut == base->second.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  base->second.erase(debut);
  out << "Debut " << key << " removed from base " << baseName;
}

void finaev::force_delete(std::istream& in, std::ostream& out, globalDebuts& debuts, debutsBases& bases)
{
  std::string key, temp;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  for (auto it = bases.begin(); it != bases.end(); ++it)
  {
    it->second.erase(key);
  }
  debuts.erase(key);
  out << "Debut " << key << " completely deleted from system debuts";
}

void finaev::safe_delete(std::istream& in, std::ostream& out, globalDebuts& debuts, const debutsBases& bases)
{
  std::string key, temp;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  bool inBase = false;
  for (auto it = bases.cbegin(); it != bases.cend(); ++it)
  {
    if (it->second.find(key) != it->second.cend())
    {
      inBase = true;
      break;
    }
  }
  if (inBase)
  {
    out << "Cannot delete " << key << " exists in one or more bases";
    return;
  }
  debuts.erase(key);
  out << "Debut " << key << " safely deleted from system";
}

void finaev::delete_debut(std::istream& in, std::ostream& out, globalDebuts& debuts, debutsBases& bases)
{
  std::string key, temp;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto debut = debuts.find(key);
  if (debut == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  size_t count = 0;
  for (auto it = bases.begin(); it != bases.end(); ++it)
  {
    if (it->second.find(key) != it->second.end())
    {
      ++count;
    }
  }
  if (count > 0)
  {
    out << "Are you sure? The debut is contained in " << count << " bases [y/n]: ";
    char answer;
    in >> answer;
    if (answer != 'y' && answer != 'Y')
    {
      throw std::runtime_error("Deletion cancelled");
    }
    for (auto it = bases.begin(); it != bases.end(); ++it)
    {
      it->second.erase(key);
    }
  }
  debuts.erase(debut);
  out << "Debut " << key << (count > 0 ? " completely deleted from system debuts" : " safely deleted from system");
}


void finaev::validate(std::istream& in, std::ostream& out, const globalDebuts& debuts)
{
  std::string key, temp;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto debut = debuts.find(key);
  if (debut == debuts.cend())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  bool isValidate = areAllMovesValid(debut->second.moves);
  out << key << (isValidate ? " - ok" : " - uncorrect");
}

void finaev::guess(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName, temp;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto base = bases.find(baseName);
  if (base == bases.cend())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  std::vector< std::string > debutsInBase;
  const auto& baseContents = base->second;
  for (auto it = baseContents.cbegin(); it != baseContents.cend(); ++it)
  {
    debutsInBase.push_back(it->first);
  }
  if (debutsInBase.size() < 4)
  {
    throw std::runtime_error("Not enough debuts in base");
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  for (size_t i = debutsInBase.size() - 1; i > 0; --i)
  {
    std::uniform_int_distribution< size_t > dist(0, i);
    size_t j = dist(gen);
    std::swap(debutsInBase[i], debutsInBase[j]);
  }
  debutsInBase.resize(4);
  std::uniform_int_distribution<> distr(0, 3);
  const size_t correctIndex = distr(gen);
  std::string correctKey = debutsInBase[correctIndex];
  const auto& correctDebut = debuts.at(correctKey);
  out << "Guess debut:\n";
  for (size_t i = 0; i < correctDebut.moves.moves.size(); ++i)
  {
    out << correctDebut.moves.moves[i];
    if (i != correctDebut.moves.moves.size() - 1)
    {
      out << " ";
    }
  }
  out << "\n";
  for (size_t i = 0; i < debutsInBase.size(); ++i)
  {
    const auto& currentDebut = debuts.at(debutsInBase[i]);
    out << (i + 1) << ") " << currentDebut.name << "\n";
  }
  out << "Choise 1-4: ";
  out.flush();
  int choice;
  if (!(in >> choice) || choice < 1 || choice > 4)
  {
    throw std::runtime_error("Invalid choice");
  }
  const std::string& selectedKey = debutsInBase[choice - 1];
  if (selectedKey == correctKey)
  {
    out << "True! is " << correctDebut.name << ". " << correctDebut.description;
  }
  else
  {
    out << "Not true! is " << correctDebut.name << ". " << correctDebut.description;
  }
}

finaev::mapOfCommands finaev::createCommandsHandler(std::istream& in, std::ostream& out, globalDebuts& debuts, debutsBases& bases)
{
  mapOfCommands commands;
  commands["create_debut"] = std::bind(finaev::create_debut, std::ref(in), std::ref(out), std::ref(debuts));
  commands["create_base"] = std::bind(finaev::create_base, std::ref(in), std::ref(out), std::ref(bases));
  commands["add"] = std::bind(finaev::add, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["exact_find"] = std::bind(finaev::exact_find, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  commands["find"] = std::bind(finaev::find, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  commands["print"] = std::bind(finaev::print, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  commands["move"] = std::bind(finaev::move, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["merge"] = std::bind(finaev::merge, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["intersect"] = std::bind(finaev::intersect, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["complement"] = std::bind(finaev::complement, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["remove"] = std::bind(finaev::remove, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["force_delete"] = std::bind(finaev::force_delete, std::ref(in), std::ref(out), std::ref(debuts), std::ref(bases));
  commands["safe_delete"] = std::bind(finaev::safe_delete, std::ref(in), std::ref(out), std::ref(debuts), std::cref(bases));
  commands["delete"] = std::bind(finaev::delete_debut, std::ref(in), std::ref(out), std::ref(debuts), std::ref(bases));
  commands["validate"] = std::bind(finaev::validate, std::ref(in), std::ref(out), std::cref(debuts));
  commands["guess"] = std::bind(finaev::guess, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  return commands;
}
