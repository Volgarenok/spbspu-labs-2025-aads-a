#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

#include <ArrayBuffer.hpp>
#include <exception>
#include <limits>

namespace gavrilova {

  template < typename Key, typename Value >
  struct Collector {
    Collector();
    void operator()(const std::pair< Key, Value >& node);

    long long getSum() const;
    const ArrayBuffer< Value >& getBuffer() const;

  private:
    long long int sum_;
    ArrayBuffer< Value > buffer_;
  };

  template < typename Key, typename Value >
  Collector< Key, Value >::Collector():
    sum_(0),
    buffer_()
  {}

  template < typename Key, typename Value >
  void Collector< Key, Value >::operator()(const std::pair< Key, Value >& node)
  {
    if ((node.first > 0 && sum_ > std::numeric_limits< long long int >::max() - node.first)) {
      throw std::overflow_error("<OVERFLOW>");
    } else if ((node.first < 0 && sum_ < std::numeric_limits< long long int >::min() - node.first)) {
      throw std::underflow_error("UNDERFLOW");
    }
    sum_ += node.first;
    buffer_.push_back(node.second);
  }

  template < typename Key, typename Value >
  long long Collector< Key, Value >::getSum() const
  {
    return sum_;
  }

  template < typename Key, typename Value >
  const ArrayBuffer< Value >& Collector< Key, Value >::getBuffer() const
  {
    return buffer_;
  }

}
#endif
