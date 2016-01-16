#ifndef CXX_UTIL_DEMANGLE_HPP_
#define CXX_UTIL_DEMANGLE_HPP_

/**
 * source: http://stackoverflow.com/a/4541470/840382
 */

#if defined(__GNUG__)
  #include <cstdlib>
  #include <cxxabi.h>
#endif
#include <string>

namespace cxx
{

static std::string demangle(const char *mangled)
{
#if defined(__GNUG__)
    int status = -4; // some arbitrary value to eliminate the compiler warning
    std::unique_ptr<char, decltype(&std::free)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        &std::free
    };
    return (0 == status) ? res.get() : name;
#else
    return mangled;
#endif
}

} // namespace cxx

#endif // CXX_UTIL_DEMANGLE_HPP_
