#include "access.hpp"

#include <string>

namespace std
{

/**
 * filesystem_mode
 */
template<filesystem::detail::filesystem_mode_e mode>
string to_string(filesystem::detail::filesystem_mode<mode>)
{
  constexpr char const chrs[] = {
    bittest(mode, decltype(mode)::read) ? 'r' : '-',
    bittest(mode, decltype(mode)::write) ? 'w' : '-',
    bittest(mode, decltype(mode)::execute) ? 'x' : '-',
    '\0'
  };
  return { chrs };
}

} // namespace std
