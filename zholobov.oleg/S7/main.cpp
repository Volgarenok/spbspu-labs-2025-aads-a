#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Error in parameters. Run as ./lab <filename>\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  return 0;
}
