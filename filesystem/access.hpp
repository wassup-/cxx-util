#ifndef FILESYSTEM_ACCESS_HPP_
#define FILESYSTEM_ACCESS_HPP_

#include <type_traits>

namespace filesystem
{

namespace detail
{

template<typename Enum, typename = std::enable_if_t<std::is_enum<Enum>::value>>
typename std::underlying_type<Enum>::type underlying_value(Enum e)
{ return static_cast<typename std::underlying_type<Enum>::type>(e); }

enum class filesystem_user_e : int {
  all   = 0,
  user  = 1,
  group = 2,
  other = 4,
};

enum class filesystem_mode_e : int {
  none    = 0,
  read    = 1,
  write   = 2,
  execute = 4,
};

filesystem_mode_e operator|(filesystem_mode_e left, filesystem_mode_e right)
{ return static_cast<filesystem_mode_e>(underlying_value(left) | underlying_value(right)); }

template<filesystem_user_e user>
struct filesystem_user : std::integral_constant<filesystem_user_e, user> { };

template<filesystem_mode_e mode>
struct filesystem_mode : std::integral_constant<filesystem_mode_e, mode>
{
  template<filesystem_mode_e other_mode>
  constexpr filesystem_mode<(mode | other_mode)> operator|(filesystem_mode<other_mode>) { return { }; }
};

} // namespace detail

namespace users
{

constexpr static detail::filesystem_user<detail::filesystem_user_e::all>   all   { };
constexpr static detail::filesystem_user<detail::filesystem_user_e::user>  user  { };
constexpr static detail::filesystem_user<detail::filesystem_user_e::group> group { };
constexpr static detail::filesystem_user<detail::filesystem_user_e::other> other { };

} // namespace users

namespace modes
{

constexpr static detail::filesystem_mode<detail::filesystem_mode_e::none>    none    { };
constexpr static detail::filesystem_mode<detail::filesystem_mode_e::read>    read    { };
constexpr static detail::filesystem_mode<detail::filesystem_mode_e::write>   write   { };
constexpr static detail::filesystem_mode<detail::filesystem_mode_e::execute> execute { };

} // namespace modes

template<typename User, typename Mode>
struct filesystem_access
{
  constexpr static detail::filesystem_user<User::value> user() { return { }; }
  constexpr static detail::filesystem_mode<Mode::value> mode() { return { }; }
};

} // namespace filesystem

#endif
