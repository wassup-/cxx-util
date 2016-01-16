#include "bus_iterator.hpp"

#include <iostream>
#include <iterator>
#include <string>

int main(int, char**)
{
  using iterator = hardware::usb::bus_iterator;

  const std::string strings[] = {
    "/dev/bus/usb",
    "/proc/bus/usb"
  };

  for(auto &&str : strings)
  {
    iterator begin = iterator(str), end;
    std::copy(begin, end, std::ostream_iterator<std::string>(std::cout, "\n"));
    std::cout << std::endl;
  }

}
