#include <iostream>
#include <vector>
#include "CrossReferenceSystem.h"

using namespace hismatova;
std::vector< std::string > tokenize_command_line(const std::string& command_line)
{
  std::vector< std::string > tokens;
  std::string token;
  bool in_quotes = false;
  std::string quoted_data;
  for (size_t i = 0; i < command_line.length(); i++)
  {
    char c = command_line[i];
    if (c == '"')
    {
      in_quotes = !in_quotes;
      if (!in_quotes && !quoted_data.empty())
      {
        tokens.push_back(quoted_data);
        quoted_data.clear();
      }
    }
    else if (in_quotes)
    {
      quoted_data += c;
    }
    else if (c == ' ')
    {
      if (!token.empty())
      {
        tokens.push_back(token);
        token.clear();
      }
    }
    else
    {
      token += c;
    }
  }
  if (!token.empty())
  {
    tokens.push_back(token);
  }
  if (!quoted_data.empty())
  {
    tokens.push_back(quoted_data);
  }
  return tokens;
}

void run_interactive_mode(CommandProcessor& processor)
{
  std::string command_line;
  while (true)
  {
    std::cout << "> ";
    if (!std::getline(std::cin, command_line))
    {
      break;
    }
    if (command_line.empty())
    {
      continue;
    }
    std::vector< std::string > tokens = tokenize_command_line(command_line);
    if (tokens.empty())
    {
      continue;
    }
    const std::string_view command = tokens[0];
    if (command == "exit")
    {
      break;
    }
    else
    {
      processor.process_command(tokens);
    }
  }
}

int main(int argc, char* argv[])
{
  CrossReferenceSystem system;
  CommandProcessor processor(&system);
  CommandLineParser::ParsedArgs args = CommandLineParser::parse(argc, argv, system);
  if (!args.success)
  {
    std::cout << "Use --help for usage information\n";
    return 1;
  }
  if (args.show_help)
  {
    print_help();
    return 0;
  }
  if (args.should_exit)
  {
    return 0;
  }
  print_welcome();
  run_interactive_mode(processor);
  return 0;
}
