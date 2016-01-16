#ifndef CXX_UTIL_BUS_ENUMERATOR_HPP
#define	CXX_UTIL_BUS_ENUMERATOR_HPP

#include "../../collections/linked_list.hpp"

#include <usb.h>

namespace hardware
{

namespace usb
{

typedef collections::linked_list<usb_bus> bus_enumerator;

} // namespace usb

} // namespace hardware

#endif	/* CXX_UTIL_BUS_ENUMERATOR_HPP */
