#ifndef CXX_UTIL_BUS_HPP
#define	CXX_UTIL_BUS_HPP

#include "device.hpp"

#include <string>
#include <vector>

namespace hardware
{

namespace usb
{

struct basic_bus
{
public:
    typedef basic_bus this_type;

public:
    basic_bus(std::string const & /* name */, std::string const & /* path */);
    ~basic_bus();

    std::string const & name() const;
    std::string path() const;

    std::vector<basic_device> devices() const;

    static std::vector<this_type> all();

private:
    std::string m_path;
    std::string m_name;
};

std::vector<basic_bus> get_all_busses();

} // namespace usb

} // namespace hardware

#endif // CXX_UTIL_BUS_HPP
