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
    outputListWithNewLine(std::cout, names);
    auto isEmptyList = [](const auto& list) -> bool
    {
      return list.empty();
    };
    if (names.empty() || std::all_of(numbers.begin(), numbers.end(), isEmptyList)) {
      std::cout << "0\n";
      return 0;
    }
    ForwardList< NumbersT::value_type > sums;
    for (const auto& num : numbers) {
      outputListWithNewLine(std::cout, num);
      sums.pushBack(safeAccumulate(num));
    }
    outputListWithNewLine(std::cout, sums);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}

