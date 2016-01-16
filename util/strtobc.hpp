#ifndef CXX_UTIL_STRTOBC_HPP_
#define CXX_UTIL_STRTOBC_HPP_

#include <string>

namespace util
{

inline long strtobc(std::string str, bool exact = true)
{
  char* next = 0;
  long cnt = std::strtol(str.c_str(), &next, 10);
  if(cnt <= 0) return 0;
  switch(*next)
  {
    // terrabytes
    case 't': cnt *= 1024;  /* fall-through */
    // gigabytes
    case 'g': cnt *= 1024;  /* fall-through */
    // megabytes
    case 'm': cnt *= 1024;  /* fall-through */
    // kilobytes
    case 'k': cnt *= 1024;  /* fall-through */
    // bytes
    case 'b':               /* fall-through */
    // end-of-string
    case '\0':              /* fall-through */
    // unsupported
    default: return cnt;
  }
}

inline std::string bctostr(long bc, bool exact = true)
{
  using std::to_string;

  // in exact mode, if not a multiple of 1024, we assume count in bytes
  if(exact && (bc % 1024) != 0) {
    // bytes
    return to_string(bc) + "b";
  }
  const long times = bc / 1024;
  if(times >= (1024 * 1024 * 1024)) {
    return to_string(times / (1024 * 1024 * 1024)) + "g";
  } else if(times >= (1024 * 1024)) {
    return to_string(times / (1024 * 1024)) + "g";
  } else if(times >= 1024) {
    // megabytes
    return to_string(times / 1024) + "m";
  } else if(times > 0) {
    // kilobytes
    return to_string(times) + "k";
  } else {
    // bytes
    return to_string(times) + "b";
  }
}

} // namespace util

#endif // CXX_UTIL_STRTOBC_HPP_
