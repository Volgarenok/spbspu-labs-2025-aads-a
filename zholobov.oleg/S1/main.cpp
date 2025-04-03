#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "CircularFwdList.hpp"

namespace {

  template < typename T >
  class SequencePrinter {
  public:
    explicit SequencePrinter(std::ostream& out, std::string delimiter = " ") noexcept:
      out_(out), delimiter_(std::move(delimiter)), first_(true) {}

    SequencePrinter(const SequencePrinter&) = delete;
    SequencePrinter& operator=(const SequencePrinter&) = delete;

    SequencePrinter& operator<<(const T& value)
    {
      if (!first_) {
        out_ << delimiter_;
      }
      first_ = false;
      out_ << value;
      return *this;
    }

  private:
    std::ostream& out_;
    std::string delimiter_;
    bool first_;
  };

  using ElemType = unsigned long;
  using ElemList = zholobov::CircularFwdList< ElemType >;
  using NamedElemList = std::pair< std::string, ElemList >;
  using ListOfNamedElemList = zholobov::CircularFwdList< NamedElemList >;

  ListOfNamedElemList read_list_of_elems_list(std::istream& input)
  {
    ListOfNamedElemList list;
    while (input.good()) {
      std::string line;
      std::getline(input, line);
      std::stringstream ss(line);
      std::string name;
      ElemList int_list;
      if (!(ss >> name)) {
        continue;
      }
      ElemType elem = 0;
      while (ss >> elem) {
        int_list.push_back(elem);
      }
      NamedElemList list_elem(std::move(name), std::move(int_list));
      list.push_back(std::move(list_elem));
    }
    return list;
  }

  int calc_and_print_result(std::ostream& out, const ListOfNamedElemList& list)
  {
    ElemList sum_list;
    bool is_overflow = false;
    zholobov::CircularFwdList< std::pair< ElemList::const_iterator, ElemList::const_iterator > > iter_list;
    if (!list.empty()) {
      SequencePrinter< std::string > printer(out);
      for (const auto& elem : list) {
        printer << elem.first;
        iter_list.push_back(std::make_pair(elem.second.cbegin(), elem.second.cend()));
      }
      out << "\n";
    }
    bool is_done_printing = true;
    do {
      is_done_printing = true;
      ElemType sum = 0;
      SequencePrinter< ElemType > printer(out);
      for (auto& elem : iter_list) {
        if (elem.first != elem.second) {
          ElemType val = *(elem.first);
          printer << val;
          ++elem.first;
          is_done_printing = false;
          if (sum <= std::numeric_limits< ElemType >::max() - val) {
            sum += val;
          } else {
            is_overflow = true;
          }
        }
      }
      if (!is_done_printing) {
        out << "\n";
        sum_list.push_back(sum);
      }
    } while (!is_done_printing);

    if (is_overflow) {
      std::cerr << "Overflow error\n";
      return 1;
    }

    if (sum_list.empty()) {
      out << "0\n";
    } else {
      SequencePrinter< ElemType > printer(out);
      for (const auto& elem : sum_list) {
        printer << elem;
      }
      out << "\n";
    }
    return 0;
  }

}

int main()
{
  int exit_code = 0;
  try {
    ListOfNamedElemList list = read_list_of_elems_list(std::cin);
    exit_code = calc_and_print_result(std::cout, list);
  } catch (const std::bad_alloc& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return exit_code;
}
