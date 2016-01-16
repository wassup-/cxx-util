#include "device.hpp"
#include "bus.hpp"

#include <usb.h>

namespace hardware
{

namespace usb
{

basic_device::basic_device(basic_bus const * b, std::string const & p)
: m_bus(b), m_path(p)
{ }

basic_device::~basic_device()
{ }

basic_bus const & basic_device::bus() const {
    return *m_bus;
}

std::string const & basic_device::name() const {
    return m_path;
}

std::string basic_device::path() const {
    return m_bus->path() + "/" + m_path;
}

std::vector<basic_device> basic_device::subdevices() const {
    std::vector<basic_device> ret;
    return ret;
}

} // namespace usb

} // namespace hardware
