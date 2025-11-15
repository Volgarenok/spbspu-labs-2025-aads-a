#include "openningInfo.hpp"
#include <iterator>
#include <iostream>
#include <numeric>

std::istream& finaev::operator>>(std::istream& in, DebutMoves& m)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char quote;
  in >> quote;
  if (quote != '"')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  DynamicArr< std::string > moves;
  std::string currentMove;
  while (in >> currentMove)
  {
    size_t quotePos = currentMove.find('"');
    if (quotePos != std::string::npos)
    {
        if (quotePos > 0)
        {
        moves.push(currentMove.substr(0, quotePos));
        }
        break;
    }
    moves.push(currentMove);
  }
  if (in)
  {
    m.moves = std::move(moves);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool finaev::DebutMoves::containsSequence(const DynamicArr< std::string >& sequence) const
{
  if (sequence.empty())
  {
    return false;
  }
  for (size_t i = 0; i <= moves.size() - sequence.size(); ++i)
  {
    bool match = true;
    for (size_t j = 0; j < sequence.size(); ++j)
    {
      if (moves[i + j] != sequence[j])
      {
        match = false;
        break;
      }
    }
    if (match)
    {
      return true;
    }
  }
  return false;
}
