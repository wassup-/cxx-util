#ifndef CXX_UTIL_DIVROUNDUP_HPP_
#define CXX_UTIL_DIVROUNDUP_HPP_

#include <limits>       // for std::numeric_limits
#include <type_traits>  // for std::is_signed, std::false_type, std::true_type

namespace math
{

namespace detail
{

using True = std::true_type;
using False = std::false_type;

template<typename T, typename U>
inline T divroundup2(T x, U y, T z, True /* signed result */)
{
  const bool divided_evenly = (x % y) == 0;
  if(divided_evenly) { return z; }
  const bool was_rounded_down = ((x > 0) == (y > 0));
  return (z + was_rounded_down);
}

template<typename T, typename U>
inline T divroundup2(T x, U y, T z, False /* unsigned result */)
{
  const bool divided_evenly = (x % y) == 0;
  return (z + !divided_evenly);
}

/**
 * Specifications:
 * - throws if y = -1 and x = std::numeric_limits<T>::minimum
*/
template<typename T, typename U>
inline T divroundup1(T x, U y, True /* T signed */, True /* Y signed */)
{
  if((y == -1) && (x == std::numeric_limits<T>::min())) { throw y; }
  return divroundup2(x, y, (x / y), True());
}

template<typename T, typename U>
inline T divroundup1(T x, U y, False /* T unsigned */, False /* Y unsigned */)
{
  return divroundup2(x, y, (x / y), False());
}

template<typename T, typename U>
inline T divroundup1(T x, U y, True /* T signed */, False /* Y unsigned */)
{
  const bool positive = (x >= 0);
  if(positive) {
    return divroundup2(x, y, (x / y), False());
  } else {
    return divroundup2(x, y, (x / y), True());
  }
}

template<typename T, typename U>
inline T divroundup1(T x, U y, False /* T unsigned */, True /* Y signed */)
{
  const bool positive = (y >= 0);
  if(positive) {
    return divroundup2(x, y, (x / y), False());
  } else {
    return divroundup2(x, y, (x / y), True());
  }
}

} // namespace detail

/**
 * Divide and round up if not evenly divisible
 * Specifications:
 * - throws if y = 0
*/
template<typename T, typename U>
inline T divroundup(T x, U y)
{
  if(y == 0) { throw y; }
  return detail::divroundup1(x, y, std::is_signed<T>(), std::is_signed<U>());
}

} // namespace math

#endif // CXX_UTIL_DIVROUNDUP_HPP_
