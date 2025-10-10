#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include <set>
#include <hashtable.hpp>
#include <vector.hpp>

namespace kushekbaev
{
  using dictionary_system = HashTable< std::string, HashTable< std::string, Vector < std::string > > >;

  void insert(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void insert_without_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void print(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void remove(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void save(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void import_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void clear_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void reverse_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void remove_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void remove_translation_at_all(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void delete_all_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void prefix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void no_prefix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void suffix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void no_suffix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void merge(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void split(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void find_words_without_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void complement(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void intersect(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void unification(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);
}

#endif
