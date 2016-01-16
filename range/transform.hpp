#ifndef CXX_UTIL_RANGE_TRANSFORM_HPP_
#define CXX_UTIL_RANGE_TRANSFORM_HPP_

#include "../functional/apply.hpp"

namespace range
{

template<typename InputIter, typename OutputIter, typename Functor>
void transform_apply(InputIter begin, InputIter end, OutputIter out, Functor &&fn)
{
  while(begin != end)
  {
    *out++ = functional::apply(fn, *begin++);
  }
}

} // namespace range

#endif // CXX_UTIL_RANGE_TRANSFORM_HPP_
