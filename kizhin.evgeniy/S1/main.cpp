#include <algorithm>
#include <stdexcept>
#include "io-utils.hpp"
#include "list-utils.hpp"

int main()
{
  using namespace kizhin;
  try {
    NamesT names;
    ForwardList< NumbersT > numbers;
    if (!inputSequences(std::cin, names, numbers)) {
      throw std::logic_error("Failed to input");
    }
    numbers = transpose(numbers);
    outputList(std::cout, names);
    if (!names.empty()) {
      std::cout << '\n';
    }
    if (names.empty() || std::all_of(numbers.begin(), numbers.end(), is_empty_list{})) {
      std::cout << "0\n";
      return 0;
    }
    ForwardList< NumbersT::value_type > sums;
    for (const auto& num: numbers) {
      outputList(std::cout, num);
      if (!num.empty()) {
        std::cout << '\n';
      }
      sums.pushBack(safeAccumulate(num));
    }
    outputList(std::cout, sums);
    if (!sums.empty()) {
      std::cout << '\n';
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}

