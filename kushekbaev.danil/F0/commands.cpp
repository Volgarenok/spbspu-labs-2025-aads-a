#include "commands.hpp"
#include <stdexcept>
#include <functional>
#include <fstream>

using namespace kushekbaev;
using dictionary_system = kushekbaev::HashTable< std::string, kushekbaev::HashTable< std::string, kushekbaev::Vector< std::string > > >;

namespace
{
  kushekbaev::Vector< std::string > split_line(std::string& line)
  {
    Vector< std::string > tokens;
    struct Splitter
    {
      std::string& line;
      size_t pos = 0;
      bool operator()(std::string& token)
      {
        size_t start = line.find_first_not_of(" \t", pos);
        if (start == std::string::npos) return false;
        size_t end = line.find_first_of(" \t", start);
        if (end == std::string::npos) end = line.size();
        token = line.substr(start, end - start);
        pos = end;
        return true;
      }
    };
    Splitter splitter{ line };
    std::string token;
    while (splitter(token))
    {
      tokens.pushBack(token);
    }
    return tokens;
  }
}

void kushekbaev::insert(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, inputed_word, inputed_translation;
  in >> dictionary_name >> inputed_word >> inputed_translation;
  auto& word_map = current_dictionary_system[dictionary_name];
  auto& translation_set = word_map[inputed_word];
  if (translation_set.count(inputed_translation))
  {
    throw std::logic_error("<THIS TRANSLATION ALREADY EXISTS>");
  }
  translation_set.insert(inputed_translation);
  out << std::string("Translation inserted successfully.\n");
}

void kushekbaev::insert_without_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, inputed_word;
  in >> dictionary_name >> inputed_word;
  auto& word_map = current_dictionary_system[dictionary_name];
  word_map[inputed_word];
  out << "Successfully inserted without translation!\n";
}

void kushekbaev::print(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name;
  in >> dictionary_name;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
      throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  out << "In " << dictionary_name << ":\n";
  for (const auto& word_entry: word_map)
  {
    const std::string& word = word_entry.first;
    const Vector< std::string >& translations = word_entry.second;
    out << "  -> " << word << " : ";
    if (translations.empty())
    {
      out << "<NO TRANSLATIONS>";
    }
    else
    {
      auto it = translations.begin();
      out << *it;
      ++it;
      for (; it != translations.end(); ++it)
      {
        out << ", " << *it;
      }
    }
    out << "\n";
  }
}

void kushekbaev::remove(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_remove;
  in >> dictionary_name >> word_to_remove;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto word_it = dict_it->second.find(word_to_remove);
  if (word_it == dict_it->second.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  dict_it->second.erase(word_it);
  out << "Word with its translation successfully removed.\n";
}

void kushekbaev::save(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string filename;
  in >> filename;
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open/create your file!");
  }
  for (const auto& dict_pair: current_dictionary_system)
  {
    const std::string& dict_name = dict_pair.first;
    const auto& word_map = dict_pair.second;
    file << "[ " << dict_name << " ]\n";
    for (const auto& word_pair: word_map)
    {
      const std::string& word = word_pair.first;
      const Vector< std::string >& translations = word_pair.second;
      file << word;
      for (const auto& translation: translations)
      {
        file << " " << translation;
      }
      file << "\n";
    }
    file << "\n";
  }
  out << "Dictionary system successfully saved.\n";
}

void kushekbaev::import_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string filename;
  in >> filename;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open your file!");
  }
  std::string line;
  std::string current_dictionary_system_name;
  size_t line_count = 0;
  bool in_dictionary = false;
  while (std::getline(file, line))
  {
    ++line_count;
    size_t start_pos = line.find_first_not_of(" \r\n");
    if (start_pos == std::string::npos)
    {
      in_dictionary = false;
      continue;
    }
    size_t end_pos = line.find_last_not_of(" \r\n");
    std::string trimmed_line = line.substr(start_pos, end_pos - start_pos + 1);
    if (trimmed_line.size() > 2 && trimmed_line[0] == '[' && trimmed_line[trimmed_line.size() - 1] == ']')
    {
      size_t start = 1;
      size_t end = trimmed_line.size() - 1;
      current_dictionary_system_name = trimmed_line.substr(start, end - start);
      start_pos = current_dictionary_system_name.find_first_not_of(" ");
      end_pos = current_dictionary_system_name.find_last_not_of(" ");
      if (start_pos != std::string::npos)
      {
        current_dictionary_system_name = current_dictionary_system_name.substr(start_pos, end_pos - start_pos + 1);
      }
      if (current_dictionary_system_name.empty())
      {
        throw std::runtime_error("Empty dictionary name at line " + std::to_string(line_count));
      }
      in_dictionary = true;
      continue;
    }
    if (in_dictionary && !current_dictionary_system_name.empty())
    {
      Vector< std::string > tokens = split_line(trimmed_line);
      if (tokens.empty())
      {
        continue;
      }
      std::string word = tokens[0];
      Vector< std::string > translations;
      for (size_t i = 1; i < tokens.size(); i++)
      {
        translations.insert(tokens[i]);
      }
      auto& existing_translations = current_dictionary_system[current_dictionary_system_name][word];
      existing_translations.insert(translations.begin(), translations.end());
    }
  }
  out << "Successfully imported file.\n";
}

void kushekbaev::search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_find;
  in >> dictionary_name >> word_to_find;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  auto word_it = word_map.find(word_to_find);
  if (word_it == word_map.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  auto& translations = word_it->second;
  out << word_it->first << std::string(" ") << *translations.begin();
  auto translation_it = translations.begin();
  ++translation_it;
  for (; translation_it != translations.end(); ++translation_it)
  {
    out << std::string(" ") << *translation_it;
  }
}

void kushekbaev::clear_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name;
  in >> dictionary_name;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  current_dictionary_system.erase(dict_it);
  out << "Dictionary successfully deleted\n";
}

void kushekbaev::reverse_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_find;
  in >> dictionary_name >> translation_to_find;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  Vector< std::string > matching_words;
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    const Vector< std::string >& translations = word_pair.second;
    if (translations.find(translation_to_find) != translations.end())
    {
      matching_words.pushBack(word);
    }
  }
  if (matching_words.empty())
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  else
  {
    out << "Words with translation *" << translation_to_find << "*: " << matching_words[0];
    for (size_t i = 1; i < matching_words.size(); ++i)
    {
      out << std::string(", ") << matching_words[i];
    }
    out << "\n";
  }
}

void kushekbaev::remove_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_find, translation_to_delete;
  in >> dictionary_name >> word_to_find >> translation_to_delete;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  auto word_it = word_map.find(word_to_find);
  if (word_it == word_map.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  auto& translations = word_it->second;
  auto translation_it = translations.find(translation_to_delete);
  if (translation_it == translations.end())
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  translations.erase(translation_it);
  out << "Translation successfully deleted.\n";
}

void kushekbaev::remove_translation_at_all(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_delete;
  in >> dictionary_name >> translation_to_delete;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  Vector< std::string > words_to_erase;
  size_t removed_count = 0;
  for (auto& word_pair: dict_it->second)
  {
    auto& translations = word_pair.second;
    if (translations.erase(translation_to_delete))
    {
      removed_count++;
      if (translations.empty())
      {
        words_to_erase.pushBack(word_pair.first);
      }
    }
  }
  for (const auto& word: words_to_erase)
  {
    dict_it->second.erase(word);
  }
  if (removed_count == 0)
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  out << "Removed " << removed_count << " instances of translation '" << translation_to_delete << ".'\n";
}
void kushekbaev::delete_all_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_delete;
  in >> dictionary_name >> translation_to_delete;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  Vector< std::string > matching_words;
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    const Vector< std::string >& translations = word_pair.second;
    if (translations.find(translation_to_delete) != translations.end())
    {
      matching_words.pushBack(word);
    }
  }
  if (matching_words.empty())
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  else
  {
    for (size_t i = 0; i < matching_words.size(); ++i)
    {
      dict_it->second.erase(matching_words[i]);
    }
  }
  out << "Words with this translation deleted successfully.\n";
}

void kushekbaev::prefix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, prefix;
  in >> dictionary_name >> prefix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found = false;
  out << "Words with prefix '" << prefix << "':\n";
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    if (word.rfind(prefix, 0) == 0)
    {
      out << "-> " << word << " : ";
      const auto& translations = word_pair.second;
      for (auto it = translations.begin(); it != translations.end(); ++it)
      {
        if (it != translations.begin()) out << ", ";
        out << *it;
      }
      out << "\n";
      found = true;
    }
  }
  if (!found)
  {
    out << "<NO WORDS FOUND>\n";
  }
}

void kushekbaev::no_prefix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, prefix;
  in >> dictionary_name >> prefix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found = false;
  out << "Words without prefix '" << prefix << "':\n";
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    if (word.rfind(prefix, 0) != 0)
    {
      out << "-> " << word << " : ";
      const auto& translations = word_pair.second;
      for (auto it = translations.begin(); it != translations.end(); ++it)
      {
        if (it != translations.begin()) out << ", ";
        out << *it;
      }
      out << "\n";
      found = true;
    }
  }
  if (!found)
  {
    out << "<NO WORDS FOUND>\n";
  }
}

void kushekbaev::suffix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, suffix;
  in >> dictionary_name >> suffix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found = false;
  out << "Words with suffix '" << suffix << "':\n";
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    if (word.size() >= suffix.size() && word.compare(word.size() - suffix.size(), suffix.size(), suffix) == 0)
    {
      out << "-> " << word << " : ";
      const auto& translations = word_pair.second;
      for (auto it = translations.begin(); it != translations.end(); ++it)
      {
        if (it != translations.begin()) out << ", ";
        out << *it;
      }
      out << "\n";
      found = true;
    }
  }
  if (!found)
  {
    out << "<NO WORDS FOUND>\n";
  }
}
void kushekbaev::no_suffix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, suffix;
  in >> dictionary_name >> suffix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found = false;
  out << "Words with bo suffix '" << suffix << "':\n";
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    if (word.size() >= suffix.size() && word.compare(word.size() - suffix.size(), suffix.size(), suffix) != 0)
    {
      out << "-> " << word << " : ";
      const auto& translations = word_pair.second;
      for (auto it = translations.begin(); it != translations.end(); ++it)
      {
        if (it != translations.begin()) out << ", ";
        out << *it;
      }
      out << "\n";
      found = true;
    }
  }
  if (!found)
  {
    out << "<NO WORDS FOUND>\n";
  }
}

void kushekbaev::merge(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_dictionary, first_dictionary, second_dictionary;
  in >> new_dictionary >> first_dictionary >> second_dictionary;
  auto first_dict_it = current_dictionary_system.find(first_dictionary);
  if (first_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<FIRST DICTIONARY NOT FOUND>");
  }
  auto second_dict_it = current_dictionary_system.find(second_dictionary);
  if (second_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<SECOND DICTIONARY NOT FOUND>");
  }
  if (current_dictionary_system.count(new_dictionary))
  {
    throw std::runtime_error("<NEW DICTIONARY NAME ALREADY EXISTS>");
  }
  auto& new_dict = current_dictionary_system[new_dictionary];
  if (first_dictionary == second_dictionary)
  {
    for (const auto& word_entry: first_dict_it->second)
    {
      new_dict[word_entry.first] = word_entry.second;
    }
  }
  else
  {
    for (const auto& word_entry: first_dict_it->second)
    {
      new_dict[word_entry.first] = word_entry.second;
    }

    for (const auto& word_entry: second_dict_it->second)
    {
      const std::string& word = word_entry.first;
      const Vector< std::string >& translations = word_entry.second;
      new_dict[word].insert(translations.begin(), translations.end());
    }
  }
  out << "Successfully merged!\n";
}
void kushekbaev::split(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_dictionary_1, new_dictionary_2, original_dictionary, delimiter;
  in >> new_dictionary_1 >> new_dictionary_2 >> original_dictionary >> delimiter;
  auto original_dict_it = current_dictionary_system.find(original_dictionary);
  if (original_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<ORIGINAL DICTIONARY NOT FOUND>");
  }
  if (current_dictionary_system.count(new_dictionary_1)  || current_dictionary_system.count(new_dictionary_2))
  {
    throw std::runtime_error("<NEW DICTIONARY NAME ALREADY EXISTS>");
  }
  auto& dict1 = current_dictionary_system[new_dictionary_1];
  auto& dict2 = current_dictionary_system[new_dictionary_2];
  for (const auto& entry: original_dict_it->second)
  {
    const std::string& word = entry.first;
    const Vector< std::string >& translations = entry.second;
    if (word < delimiter)
    {
      dict1[word] = translations;
    }
    else
    {
      dict2[word] = translations;
    }
  }
  out << "Successfully splitted.\n";
}

void kushekbaev::find_words_without_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary;
  in >> dictionary;
  auto dict_it = current_dictionary_system.find(dictionary);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found_empty = false;
  out << "Words without translations in dictionary '" << dictionary << "':\n";
  for (const auto& word_entry: dict_it->second)
  {
    const std::string& word = word_entry.first;
    const Vector< std::string >& translations = word_entry.second;
    if (translations.empty())
    {
      out << "-> " << word << "\n";
      found_empty = true;
    }
  }
  if (!found_empty)
  {
    out << "<ALL WORDS HAVE TRANSLATIONS>\n";
  }
}

void kushekbaev::complement(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
    std::string new_name, dict1_name, dict2_name;
    in >> new_name >> dict1_name >> dict2_name;
    auto dict1_it = current_dictionary_system.find(dict1_name);
    auto dict2_it = current_dictionary_system.find(dict2_name);
    if (dict1_it == current_dictionary_system.end() || dict2_it == current_dictionary_system.end())
    {
      throw std::out_of_range("<DICTIONARY NOT FOUND>");
    }
    if (dict1_name == dict2_name)
    {
      current_dictionary_system[new_name] = {};
      out << "Created empty dictionary (same dictionaries complemented)\n";
      return;
    }
    auto& new_dict = current_dictionary_system[new_name] = {};
    const auto& dict1 = dict1_it->second;
    const auto& dict2 = dict2_it->second;
    for (const auto& word_entry: dict1)
    {
      if (dict2.find(word_entry.first) == dict2.cend())
      {
        new_dict.insert(word_entry);
      }
    }
    out << "Complement dictionary created successfully\n";
}

void kushekbaev::intersect(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_name, dict1_name, dict2_name;
  in >> new_name >> dict1_name >> dict2_name;
  auto dict1_it = current_dictionary_system.find(dict1_name);
  auto dict2_it = current_dictionary_system.find(dict2_name);
  if (dict1_it == current_dictionary_system.end() || dict2_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& new_dict = current_dictionary_system[new_name] = {};
  const auto& dict1 = dict1_it->second;
  const auto& dict2 = dict2_it->second;
  for (const auto& word_entry: dict1)
  {
    const std::string& word = word_entry.first;
    if (dict2.count(word))
    {
      auto& translations = new_dict[word];
      translations.insert(word_entry.second.begin(), word_entry.second.end());
      translations.insert(dict2.at(word).begin(), dict2.at(word).end());
    }
  }
  out << "Intersection dictionary created successfully\n";
}

void kushekbaev::unification(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_name, dict1_name, dict2_name;
  in >> new_name >> dict1_name >> dict2_name;
  auto dict1_it = current_dictionary_system.find(dict1_name);
  auto dict2_it = current_dictionary_system.find(dict2_name);
  if (dict1_it == current_dictionary_system.end() ||  dict2_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& new_dict = current_dictionary_system[new_name] = {};
  const auto& dict1 = dict1_it->second;
  const auto& dict2 = dict2_it->second;
  for (const auto& word_entry: dict1)
  {
    new_dict[word_entry.first].insert(word_entry.second.begin(), word_entry.second.end());
  }
  for (const auto& word_entry: dict2)
  {
    new_dict[word_entry.first].insert(word_entry.second.begin(), word_entry.second.end());
  }
  out << "Unification dictionary created successfully\n";
}
