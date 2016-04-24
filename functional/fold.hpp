#ifndef CXX_UTIL_FOLD_HPP_
#define CXX_UTIL_FOLD_HPP_

#include <utility>

namespace functional
{

#define AUTO_RETURNS(x) -> decltype(x) { return (x); }

/*
 * foldl
 * eg [e, f, g] => g(f(e()))
 */
struct foldl_helper
{
  template<typename State, typename Fn>
  static auto fold(State &&s, Fn &&fn)
  AUTO_RETURNS(std::forward<Fn>(fn)(std::forward<State>(s)));

  template<typename State, typename Fn, typename ...Rest>
  static auto fold(State &&s, Fn &&fn, Rest &&...rest)
  AUTO_RETURNS(fold(fold(std::forward<State>(s), std::forward<Fn>(fn)), std::forward<Rest>(rest)...));
};

template<typename First, typename... Rest>
auto foldl(First&& first, Rest&&... rest)
AUTO_RETURNS(foldl_helper::fold(std::forward<First>(first)(), std::forward<Rest>(rest)...));

/*
 * foldr
 * eg [e, f, g] => e(f(g()))
 */
struct foldr_helper
{
  template<typename Fn>
  static auto fold(Fn &&fn)
  AUTO_RETURNS(std::forward<Fn>(fn)());

  template<typename Fn, typename ...Rest>
  static auto fold(Fn &&fn, Rest &&...rest)
  AUTO_RETURNS(std::forward<Fn>(fn)(fold(std::forward<Rest>(rest)...)));
};

template<typename First, typename... Rest>
auto foldr(First&& first, Rest&&... rest)
AUTO_RETURNS(foldr_helper::fold(std::forward<First>(first), std::forward<Rest>(rest)...));

#undef AUTO_RETURNS

} // namespace functional

#endif // CXX_UTIL_FOLD_HPP_
