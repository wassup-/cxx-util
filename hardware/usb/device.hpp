#ifndef CXX_UTIL_DEVICE_HPP
#define	CXX_UTIL_DEVICE_HPP

#include <string>
#include <vector>

namespace hardware
{

namespace usb
{

struct basic_bus;

struct basic_device
{
public:
    typedef basic_device this_type;

protected:
    const basic_bus *m_bus;
    std::string m_path;

public:
    basic_device(const basic_bus*, const std::string&);
    ~basic_device();

    const basic_bus& bus() const;

    const std::string& name() const;
    std::string path() const;

    std::vector<this_type> subdevices() const;
};

} // namespace usb

} // namespace hardware

#endif	/* CXX_UTIL_DEVICE_HPP */
