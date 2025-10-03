#ifndef KEY_SUMM_HPP
#define KEY_SUMM_HPP
#include <utility>
#include <string>

namespace demehin
{
  struct KeySumm
  {
    int val_res;
    std::string str_res;

    KeySumm() noexcept;
    void operator()(const std::pair< const int, std::string >&);

  };
}


#endif
