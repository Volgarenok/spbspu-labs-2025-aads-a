#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <string>
#include <hash_table/hash_table.hpp>

namespace abramov
{
  struct Dictionary
  {
    Dictionary();
    Dictionary(const Dictionary &dict);
    Dictionary &operator=(const Dictionary &dict);
    void addWord(const std::string &word, const std::string &trans);
    Dictionary mergeDict(const Dictionary &dict) const;
    void deleteWord(const std::string &word);
    Dictionary diffDict(const Dictionary &dict) const;
    void print(std::ostream &out) const noexcept;
    Dictionary setWithDict(const Dictionary &dict) const;
    Dictionary intersectWithDict(const Dictionary &dict) const;
    Dictionary unionWithDict(const Dictionary &dict) const;
    bool empty() const noexcept;

    friend std::ostream &operator<<(std::ostream &out, const Dictionary &dict);
  private:
    HashTable< std::string, std::string > dict_;

    void swap(Dictionary &dict) noexcept;
  };

  std::ostream &operator<<(std::ostream &out, const Dictionary &dict);
}
#endif
