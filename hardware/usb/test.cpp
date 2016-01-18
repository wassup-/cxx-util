#include "bus_iterator.hpp"

#include <iostream>
#include <iterator>
#include <string>

int main(int, char**)
{
  static const std::string strings[] = {
    "/dev/bus/usb",
    "/proc/bus/usb"
  };

  for(auto &&str : strings)
  {
    using input_iterator = hardware::usb::bus_iterator;
    using output_iterator = std::ostream_iterator<std::string>;

    std::copy(input_iterator(str), input_iterator(), output_iterator(std::cout, "\n"));
    std::cout << std::endl;
  }

}
