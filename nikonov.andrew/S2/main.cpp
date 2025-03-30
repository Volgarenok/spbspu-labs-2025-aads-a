#include <iostream>
int main(int argc, char** argv)
{
  if (argc > 1)
  {
    std::cerr << "\"One\" or \"None\" parameters expected, but " << argc << " are given!\n";
    return 1;
  }
  if (argc)
  {
    return (argv != nullptr);
    //read standard input 
  }
  //doing some file magic "Wshhhhh..Shhh..""
}
