#ifndef MANIPULATIONS_WITH_FWD_LIST_HPP
#define MANUPULATIONS_WITH_FWD_LIST_HPP

#include <iostream>
#include "forward_ring_list.hpp"

namespace petrov
{
  using list_type = ForwardRingList< std::pair< std::string, petrov::ForwardRingList< size_t > > >;
  using list_it_t = ForwardListIterator< std::pair< std::string, petrov::ForwardRingList< size_t > > >;
  using sublist_it_t = ForwardListIterator< size_t >;
  using result_list_type = ForwardRingList< size_t >;

  std::istream & inputValuesIntoFwdRingList(std::istream & in, list_type & fwd_ring_list);
  std::ostream & outputNamesOfSuquences(std::ostream & out, const list_type & fwd_ring_list);

  result_list_type getListOfSequencesFromListOfSums(list_type fwd_ring_list);
  void AddElementToSumAndPopFrontIt(size_t & sum, list_it_t it_out, sublist_it_t it);

  std::ostream & outputSums(std::ostream & out, const result_list_type sums);
}


#endif
