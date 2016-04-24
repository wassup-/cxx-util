#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <type_traits>

template<typename Enum, typename = std::enable_if_t<std::is_enum<Enum>::value>>
inline constexpr typename std::underlying_type<Enum>::type underlying_value(Enum e)
{ return static_cast<typename std::underlying_type<Enum>::type>(e); }

template<typename Enum, typename = std::enable_if_t<std::is_enum<Enum>::value>>
inline constexpr Enum bitwise_and(Enum l, Enum r)
{ return static_cast<Enum>(underlying_value(l) & underlying_value(r)); }

template<typename Enum, typename = std::enable_if_t<std::is_enum<Enum>::value>>
inline constexpr Enum bitwise_or(Enum l, Enum r)
{ return static_cast<Enum>(underlying_value(l) | underlying_value(r)); }

/**
 * Check whether or not one particular bit is set
 */
template<typename Enum, typename = std::enable_if_t<std::is_enum<Enum>::value>>
constexpr bool bittest(const Enum bits, const Enum bit)
{ return !!(underlying_value(bits) & underlying_value(bit)); }

/**
 * Check whether or not some particular bits are set
 */
template<typename Enum, typename = std::enable_if_t<std::is_enum<Enum>::value>>
constexpr bool bitstest(const Enum bits, const Enum bit)
{ return ((underlying_value(bits) & underlying_value(bit)) == underlying_value(bit)); }

#endif // UTIL_HPP_
