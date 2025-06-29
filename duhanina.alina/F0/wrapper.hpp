#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include <iostream>
#include <functional>
#include <tree.hpp>

namespace duhanina
{
  using str = std::string;

  void build_codes_wrapper(std::istream&, std::ostream&);
  void show_codes_wrapper(std::istream&, std::ostream&);
  void save_codes_wrapper(std::istream&, std::ostream&);
  void load_codes_wrapper(std::istream&, std::ostream&);
  void clear_codes_wrapper(std::istream&, std::ostream&);
  void encode_file_with_codes_wrapper(std::istream&, std::ostream&);
  void decode_file_with_codes_wrapper(std::istream&, std::ostream&);
  void encode_file_wrapper(std::istream&, std::ostream&);
  void decode_file_wrapper(std::istream&, std::ostream&);
  void compare_wrapper(std::istream&, std::ostream&);
  void suggest_encodings_wrapper(std::istream& in, std::ostream& out);
  void check_encoding_wrapper(std::istream& in, std::ostream& out);

  Tree< str, std::function< void() >, std::less< str > > initTree(std::istream& in, std::ostream& out);
}

#endif
