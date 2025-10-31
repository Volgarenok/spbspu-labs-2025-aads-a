#include "output_save.hpp"
#include <fstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace
{
  bool compareCharacters(const std::pair<size_t, karnauhova::Character>& pair, const karnauhova::Character& target)
  {
    return pair.second == target;
  }
  struct ExtractCharacter
  {
    karnauhova::Character operator()(const std::pair<size_t, karnauhova::Character>& pair) const
    {
      return pair.second;
    }
};
}
void karnauhova::saveData(AvlTree< std::string, Character >& players, AvlTree< size_t, Character >& characters)
{
  std::ofstream file;
  file.open("karnauhova.alexandra/F0/game_data_save.txt");
  auto it = players.begin();
  file << it->first << " " << (++it)->first << "\n";
  auto& target_character = (--it)->second;
  auto character = characters.end();
  for (auto iter = characters.begin(); iter != characters.end(); ++iter)
  {
    if (compareCharacters(*iter, target_character))
    {
      character = iter;
      break;
    }
  }
  file << character->first << " " << it->second.volumeHp() << " " << it->second.position << "\n";
  target_character = (++it)->second;
  character = characters.end();
  for (auto iter = characters.begin(); iter != characters.end(); ++iter)
  {
    if (compareCharacters(*iter, target_character))
    {
      character = iter;
      break;
    }
  }
  file << character->first << " " << it->second.volumeHp() << " " << it->second.position << "\n";

  using ostr_iter = std::ostream_iterator< Character >;
  std::vector< Character > data;
  data.reserve(characters.size());
  std::transform(characters.begin(), characters.end(), std::back_inserter(data), ExtractCharacter());
  std::copy(std::begin(data), std::end(data), ostr_iter(file, "\n"));
}
