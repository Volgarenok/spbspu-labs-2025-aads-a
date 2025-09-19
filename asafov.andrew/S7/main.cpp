#include "graphManager.h"
#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "args err\n";
    return 1;
  }

  asafov::GraphManager manager;
  manager.read_from_file(argv[1]);
  manager.process_commands();
  return 0;
}
