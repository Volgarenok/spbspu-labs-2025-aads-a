#include "io_containers.hpp"
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "fwd_iterator.hpp"
#include "fwd_list_node.hpp"
#include "fwd_list.hpp"

void zakirov::get_list_pair(std::istream & in, list_pair & forward_list)
{
  std::string sequence_name;
  auto fillable_iterator = forward_list.before_begin();
  while (in >> sequence_name && !in.eof())
  {
    list_ull sequence_num;
    zakirov::get_list_ull(in, sequence_num);
    forward_list.insert_after(fillable_iterator, {sequence_name, sequence_num});
    ++fillable_iterator;
    in.clear();
  }
}

void zakirov::get_list_ull(std::istream & in, list_ull & forward_list)
{
  unsigned long long int sequence_element = 0;
  auto fillable_iterator = forward_list.before_begin();
  while (in >> sequence_element)
  {
    forward_list.insert_after(fillable_iterator, sequence_element);
    ++fillable_iterator;
  }
}

void zakirov::output_result(std::ostream & out, list_pair & forward_list)
{
  if (forward_list.empty())
  {
    out << 0 << '\n';
    return;
  }

  list_iter list_iterators;
  auto iter_list = list_iterators.before_begin();
  auto pair_iter = forward_list.begin();
  unsigned long long number_sum = 0;
  size_t list_size = 0;
  size_t deleted = 0;

  for (auto i = forward_list.begin(); i != forward_list.end(); ++i, ++iter_list)
  {
    list_iterators.insert_after(iter_list, i->second.begin());
    ++list_size;
  }

  auto output_iter = forward_list.begin();
  out << output_iter->first;
  ++output_iter;
  for (; output_iter != forward_list.end(); ++output_iter)
  {
    out << ' ' << output_iter->first;
  }

  out << '\n';
  list_ull list_added;
  auto iter_added = list_added.before_begin();
  bool flag_overflow = false;

  while (deleted != list_size)
  {
    deleted = 0;
    number_sum = 0;
    iter_list = list_iterators.begin();
    pair_iter = forward_list.begin();
    while (*iter_list == pair_iter->second.end() && iter_list != list_iterators.end())
    {
      ++iter_list;
      ++pair_iter;
      ++deleted;
      if (deleted == list_size)
      {
        break;
      }
    }

    if (deleted == list_size)
    {
      break;
    }

    if (*iter_list != pair_iter->second.end())
    {
      out << **iter_list;
      number_sum += (**iter_list);
      ++(* iter_list);
    }
    else
    {
      ++deleted;
    }

    ++iter_list;
    ++pair_iter;

    for (; iter_list != list_iterators.end(); ++iter_list)
    {
      if (*iter_list != pair_iter->second.end())
      {
        if (number_sum > std::numeric_limits< unsigned long long >::max() - **iter_list)
        {
          flag_overflow = true;
        }

        out << ' ' << **iter_list;
        number_sum += **iter_list;
        ++(*iter_list);
      }
      else
      {
        ++deleted;
      }

      ++pair_iter;
    }

    if (deleted != list_size)
    {
      list_added.insert_after(iter_added, number_sum);
      ++iter_added;
      out << '\n';
    }
    number_sum = 0;
  }

  if (flag_overflow)
  {
    throw std::logic_error("Stack overflow!");
  }
  else if (list_added.empty())
  {
    out << 0 << '\n';
  }
  else
  {
    iter_added = list_added.begin();
    out << *iter_added;
    ++iter_added;
    for (; iter_added != list_added.end(); ++iter_added)
    {
      out << ' ' << *iter_added;
    }

    out << '\n';
  }
}
