#ifndef CXX_UTIL_DEMANGLE_HPP_
#define CXX_UTIL_DEMANGLE_HPP_

/**
 * source: http://stackoverflow.com/a/4541470/840382
 */

#include <cstdlib>
#include <cxxabi.h>
#include <string>

namespace cxx
{

static std::string demangle(const char *mangled)
{
    int status = -4; // some arbitrary value to eliminate the compiler warning
    std::string ret;
    char* d = abi::__cxa_demangle(mangled, NULL, NULL, &status);
    if (status == 0 && d) ret = d;
    if (d) std::free(d);
    return (status == 0) ? ret : mangled;
}

} // namespace cxx

#endif // CXX_UTIL_DEMANGLE_HPP_
