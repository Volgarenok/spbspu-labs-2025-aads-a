#include "commands.hpp"
namespace
{
  void addPlayer(std::istream& in, brevnov::League& league, std::string teamName)
  {
    std::string playerName, position;
    int rait, pr;
    in >> playerName >> position >> rait >> pr;
    if (rait > 0 && pr > 0 && brevnov::checkPosition(position))
    {
      size_t raiting = rait;
      size_t price = pr;
      auto clubFind = league.teams_.find(teamName);
      if (clubFind != league.teams_.end())
      {
        brevnov::Team& club = (*clubFind).second;
        if (club.players_.find(playerName) == club.players_.end())
        {
          std::pair< std::string, brevnov::Player > pair(playerName, brevnov::Player(position, raiting, price));
          club.players_.insert(pair);
        }
        else
        {
          std::cerr << "Player already exist!\n";
        }
      }
      else
      {
        if (league.fa_.find(playerName) == league.fa_.end())
        {
          std::pair< std::string, brevnov::Player > pair(playerName, brevnov::Player(position, raiting, price));
          league.fa_.insert(pair);
        }
        else
        {
          std::cerr << "Player already exist!\n";
        }
      }
    }
    else
    {
      std::cerr << "Not correct parameters!\n";
    }
  }

  void addT(std::istream& in, brevnov::League& league, std::string teamName)
  {
    int bud = 0;
    in >> bud;
    if (bud > 0)
    {
      size_t budget = bud;
      if (league.teams_.find(teamName) == league.teams_.end())
      {
        std::pair< std::string, brevnov::Team > pair(teamName, brevnov::Team(budget));
        league.teams_.insert(pair);
      }
      else
      {
        std::cerr << "Team already exist!\n";
      }
    }
    else
    {
      std::cerr << "Not correct budget!\n";
    }
  }

  brevnov::Position viewM(std::istream& in, std::ostream& out, brevnov::League& league)
  {
    std::string pos;
    in >> pos;
    if (!brevnov::checkPosition(pos))
    {
      std::cerr << "Not correct position!\n";
      throw std::logic_error("Not correct position!");
    }
    brevnov::Position sPos = brevnov::definePosition(pos);
    auto pl = league.fa_.begin();
    while (pl != league.fa_.end())
    {
      if ((*pl).second.position_ == sPos)
      {
        out << "FA " << (*pl).first << " " << (*pl).second << "\n";
      }
      pl++;
    }
    return sPos;
  }

  void buyP(std::ostream& out, brevnov::League& league, brevnov::Team& club, size_t bud, brevnov::Position sPos)
  {
    auto pl = league.fa_.begin();
    auto maxpl = league.fa_.end();
    size_t rait = 0;
    while (pl != league.fa_.end())
    {
      brevnov::Player& p = (*pl).second;
      if (p.position_ == sPos && p.price_ <= bud && p.raiting_ > rait)
      {
        maxpl = pl;
        rait = p.raiting_;
      }
      pl++;
    }
    if (maxpl != league.fa_.end())
    {
      club.budget_ -= (*maxpl).second.price_;
      out << "Bought " << (*maxpl).first << " " << (*maxpl).second << "\n";
      club.players_.insert(*maxpl);
      league.fa_.erase(maxpl);
    }
    else
    {
      out << "Player not found!\n";
    }
  }
}

bool brevnov::checkPosition(std::string pos)
{
  return (pos == "CF" || pos == "RF" || pos == "LF" || pos == "RB" || pos == "LB" || pos == "G");
}

void brevnov::input(std::istream& in, brevnov::League& league)
{
  std::string teamName;
  while (in >> teamName)
  {
    addT(in, league, teamName);
    if (in.get() == '\n')
      {
        break;
      }
  }
  while (in >> teamName)
  {
    addPlayer(in, league, teamName);
  }
}

void brevnov::addNewPlayer(std::istream& in, League& league)
{
  std::string teamName;
  in >> teamName;
  addPlayer(in, league, teamName);
}

void brevnov::addTeam(std::istream& in, League& league)
{
  std::string teamName;
  in >> teamName;
  addT(in, league, teamName);
}

void brevnov::deleteTeam(std::istream& in, League& league)
{
  std::string teamName;
  in >> teamName;
  auto clubFind = league.teams_.find(teamName);
  if (clubFind != league.teams_.end())
  {
    Team& club = (*clubFind).second;
    auto pl = club.players_.begin();
    while (!club.players_.empty())
    {
      league.fa_.insert((*pl));
      pl = club.players_.erase(pl);
    }
    league.teams_.erase(clubFind);
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::endCareer(std::istream& in, League& league)
{
  std::string teamName, playerName;
  in >> teamName >> playerName;
  auto clubFind = league.teams_.find(teamName);
  if (clubFind != league.teams_.end())
  {
    brevnov::Team& club = (*clubFind).second;
    if (club.players_.find(playerName) != club.players_.end())
    {
      club.players_.erase(playerName);
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
  else
  {
    if (league.fa_.find(playerName) != league.fa_.end())
    {
      league.fa_.erase(playerName);
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
}

void brevnov::updateRating(std::istream& in, League& league)
{
  std::string teamName, playerName;
  int raiting;
  in >> teamName >> playerName;
  in >> raiting;
  if (raiting <= 0)
  {
    std::cerr << "Not correct raiting!\n";
    return;
  }
  auto clubFind = league.teams_.find(teamName);
  if (clubFind != league.teams_.end())
  {
    brevnov::Team& club = (*clubFind).second;
    if (club.players_.find(playerName) != club.players_.end())
    {
      brevnov::Player& pl = (*club.players_.find(playerName)).second;
      pl.raiting_ = raiting;
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
  else
  {
    if (league.fa_.find(playerName) != league.fa_.end())
    {
      brevnov::Player& pl = (*league.fa_.find(playerName)).second;
      pl.raiting_ = raiting;
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
}

void brevnov::transfer(std::istream& in, std::ostream& out, League& league)
{
  std::string buyTeamName, soldTeamName;
  std::string playerSold;
  in >> playerSold >>  buyTeamName >> soldTeamName;
  auto buyFind = league.teams_.find(buyTeamName);
  if (buyFind != league.teams_.end())
  {
    brevnov::Team& buyTeam = (*buyFind).second;
    auto soldFind = league.teams_.find(soldTeamName);
    if (soldFind != league.teams_.end())
    {
      brevnov::Team& soldTeam = (*soldFind).second;
      if (soldTeam.players_.find(playerSold) != soldTeam.players_.end())
      {
        brevnov::Player& pl = (*soldTeam.players_.find(playerSold)).second;
        if (buyTeam.budget_ >=  pl.price_)
        {
          out << "Successful transfer: " << playerSold << " " << pl << " ";
          out << soldTeamName << " -> " << buyTeamName << "\n";
          buyTeam.budget_ -= pl.price_;
          soldTeam.budget_ += pl.price_;
          buyTeam.players_.insert(*soldTeam.players_.find(playerSold));
          soldTeam.players_.erase(soldTeam.players_.find(playerSold));
        }
      }
      else
      {
        std::cerr << "Player not found!\n";
      }
    }
    else
    {
      if (league.fa_.find(playerSold) != league.fa_.end())
      {
        brevnov::Player& pl = (*league.fa_.find(playerSold)).second;
        if (buyTeam.budget_ >=  pl.price_)
        {
          buyTeam.budget_ -= pl.price_;
          buyTeam.players_.insert(*league.fa_.find(playerSold));
          league.fa_.erase(league.fa_.find(playerSold));
        }
      }
      else
      {
        std::cerr << "Player not found!\n";
      }
    }
  }
  else
  {
    std::cerr << "BuyTeam not found!\n";
  }
}

void brevnov::buyPosition(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName, pos;
  int budh = 0;
  in >> budh;
  in >> pos >> teamName;
  if (!checkPosition(pos))
  {
    std::cerr << "Not correct position!\n";
    return;
  }
  Position sPos = definePosition(pos);
  if (budh <= 0)
  {
    std::cerr << "Not correct budget!\n";
    return;
  }
  size_t bud = budh;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    Team& club = (*findTeam).second;
    if (club.budget_ <  bud)
    {
      std::cerr << "Team have not enough money!\n";
    }
    else
    {
      buyP(out, league, club, bud, sPos);
    }
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::buyTeam(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  int budh = 0;
  in >> budh >> teamName;
  if (budh <= 0)
  {
    std::cerr << "Not correct budget!\n";
    return;
  }
  size_t bud = budh;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam == league.teams_.end())
  {
    std::cerr << "Team not found!\n";
    return;
  }
  Team& club = (*findTeam).second;
  if (club.budget_ <  bud)
  {
    std::cerr << "Team have not enough money!\n";
    return;
  }
  size_t bud_per_pos = std::max(budh / 6, 1);
  buyP(out, league, club, bud_per_pos, Position::LF);
  buyP(out, league, club, bud_per_pos, Position::RF);
  buyP(out, league, club, bud_per_pos, Position::CF);
  buyP(out, league, club, bud_per_pos, Position::LB);
  buyP(out, league, club, bud_per_pos, Position::RB);
  buyP(out, league, club, bud_per_pos, Position::G);
}

void brevnov::soldPlayer(std::istream& in, League& league)
{
  std::string teamName, playerName;
  in >> teamName >> playerName;
  auto teamFind = league.teams_.find(teamName);
  if (teamFind != league.teams_.end())
  {
    brevnov::Team& sTeam = (*teamFind).second;
    auto pl = sTeam.players_.find(playerName);
    if (pl != sTeam.players_.end())
    {
      sTeam.budget_ += (*pl).second.price_;
      league.fa_.insert(*pl);
      pl = sTeam.players_.erase(pl);
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::soldTeam(std::istream& in, League& league)
{
  std::string teamName;
  in >> teamName;
  auto teamFind = league.teams_.find(teamName);
  if (teamFind != league.teams_.end())
  {
    brevnov::Team& sTeam = (*teamFind).second;
    auto pl = sTeam.players_.begin();
    while (pl != sTeam.players_.end())
    {
      sTeam.budget_ += (*pl).second.price_;
      league.fa_.insert(*pl);
      pl++;
    }
    sTeam.players_.clear();
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::deposit(std::istream& in, League& league)
{
  std::string teamName;
  int dep;
  in >> dep >> teamName;
  if (dep <= 0)
  {
    std::cerr << "Incorrect deposit!\n";
    return;
  }
  auto teamFind = league.teams_.find(teamName);
  if (teamFind != league.teams_.end())
  {
    brevnov::Team& teamD = (*teamFind).second;
    teamD.budget_ += dep;
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::viewTeam(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  in >> teamName;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    brevnov::Team& club = (*findTeam).second;
    auto pl = club.players_.begin();
    while (pl != club.players_.end())
    {
      out << teamName << " " << (*pl).first << " " << (*pl).second << "\n";
      pl++;
    }
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::viewPosition(std::istream& in, std::ostream& out, League& league)
{
  Position sPos = Position::CF;
  try
  {
    sPos = viewM(in, out, league);
  }
  catch(const std::logic_error& e)
  {
    return;
  }
  auto club = league.teams_.begin();
  while (club != league.teams_.end())
  {
    auto pl = (*club).second.players_.begin();
    while (pl != (*club).second.players_.end())
    {
      if ((*pl).second.position_ == sPos)
      {
        out << (*club).first << " " << (*pl).first << " " << (*pl).second << "\n";
      }
      pl++;
    }
    club++;
  }
}

void brevnov::buyPlayer(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  int budh = 0;
  in >> budh >> teamName;
  if (budh <= 0)
  {
    std::cerr << "Not correct budget!\n";
  }
  size_t bud = budh;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    Team& club = (*findTeam).second;
    if (club.budget_ <  bud)
    {
      std::cerr << "Team have not enough money!\n";
    }
    else
    {
      auto pl = league.fa_.begin();
      auto maxpl = league.fa_.end();
      size_t rait = 0;
      while (pl != league.fa_.end())
      {
        Player& p = (*pl).second;
        if (p.price_ <= bud && p.raiting_ > rait)
        {
          maxpl = pl;
          rait = p.raiting_;
        }
        pl++;
      }
      if (rait != 0)
      {
        club.budget_ -= (*maxpl).second.price_;
        out << "Bought " << (*maxpl).first << " " << (*maxpl).second << "\n";
        club.players_.insert((*maxpl));
        league.fa_.erase(maxpl);
      }
      else
      {
        out << "Player not found!\n";
      }
    }
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::startTeam(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  in >> teamName;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    Team& club = (*findTeam).second;
    for (size_t i = 0; i < 6; i++)
    {
      size_t rait = 0;
      Position pos = static_cast<Position>(i);
      auto maxpl = club.players_.end();
      for (auto pl = club.players_.begin(); pl != club.players_.end(); ++pl)
      {
        if (pl->second.position_ == pos && pl->second.raiting_ > rait)
        {
          maxpl = pl;
          rait = pl->second.raiting_;
        }
      }
      if (rait > 0)
      {
        out << (*maxpl).first << " " << (*maxpl).second << "\n";
      }
      else
      {
        out << "Not player on this position!\n";
      }
    }
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::viewPlayer(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName, playerName;
  in >> teamName >> playerName;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    Team& club = (*findTeam).second;
    if (club.players_.find(playerName) != club.players_.end())
    {
      out << teamName << " " << playerName << " " << (*club.players_.find(playerName)).second << "\n";
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
  else
  {
    if (league.fa_.find(playerName) != league.fa_.end())
    {
      out << "FA " << playerName << " " << (*league.fa_.find(playerName)).second << "\n";
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
}

void brevnov::viewTeamPosition(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName, pos;
  in >> teamName >> pos;
  if (!checkPosition(pos))
  {
    std::cerr << "Not correct position!\n";
    return;
  }
  Position sPos = definePosition(pos);
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    Team& club = (*findTeam).second;
    auto pl = club.players_.begin();
    while (pl != club.players_.end())
    {
      if ((*pl).second.position_ == sPos)
      {
        out << (*findTeam).first << " " << (*pl).first << " " << (*pl).second << "\n";
      }
      pl++;
    }
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::viewMarket(std::ostream& out, League& league)
{
  auto pl = league.fa_.begin();
  while (pl != league.fa_.end())
  {
    out << "FA " << (*pl).first << " " << (*pl).second << "\n";
    pl++;
  }
}

void brevnov::viewMarketPosition(std::istream& in, std::ostream& out, League& league)
{
  try
  {
    viewM(in, out, league);
  }
  catch(const std::logic_error& e)
  {
    return;
  }
}
