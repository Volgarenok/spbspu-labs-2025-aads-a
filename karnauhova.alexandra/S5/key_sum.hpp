#ifndef KEY_SUM_HPP
#define KEY_SUM_HPP
#include <utility>
#include <string>
namespace karnauhova
{
  struct KeySum
  {
    void operator()(const std::pair< const int, std::string > & key_value);

    int result_ = 0;
    std::string itInput_;
  };
}

#endif
