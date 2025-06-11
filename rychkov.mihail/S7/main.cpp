#include <iostream>
#include <exception>
#include <parser.hpp>
#include "processor.hpp"

int main(int argc, char** argv)
{
  try
  {
    rychkov::S7ParseProcessor processor{argc, argv};
    rychkov::ParserContext context{std::cin, std::cout, std::cerr};
    while (rychkov::Parser::parse(context, processor, rychkov::S7ParseProcessor::call_map))
    {}
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
