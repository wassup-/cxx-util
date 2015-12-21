#ifndef CXX_UTIL_FOLD_HPP_
#define CXX_UTIL_FOLD_HPP_

#include <utility>

namespace functional
{

#define AUTO_RETURNS(x) -> decltype(x) { return x; }

/*
 * foldl
 * eg [e, f, g] => e(f(g()))
 */
template<typename H, typename...>
struct foldl_helper
{
  static auto fold(H fn)
  AUTO_RETURNS(fn());
};

template<typename First, typename Second, typename... Rest>
struct foldl_helper<First, Second, Rest...>
{
  using next_folder = foldl_helper<Second, Rest...>;

  static auto fold(First first, Second second, Rest... rest)
  AUTO_RETURNS(first(next_folder::fold(second, rest...)));
};

template<typename First, typename... Rest>
auto foldl(First&& first, Rest&&... rest)
AUTO_RETURNS((foldl_helper<First, Rest...>::fold(std::forward<First>(first), std::forward<Rest>(rest)...)));

/*
 * foldr
 * eg [e, f, g] => g(f(e()))
 */
template<typename H, typename...>
struct foldr_helper
{
  template<typename State>
  static auto fold(State s, H fn)
  AUTO_RETURNS(fn(s));
};

template<typename First, typename Second, typename... Rest>
struct foldr_helper<First, Second, Rest...>
{
  using next_folder = foldr_helper<Second, Rest...>;

  template<typename State>
  static auto fold(State s, First first, Second second, Rest... rest)
  AUTO_RETURNS(next_folder::fold(first(s), second, rest...));
};

template<typename First, typename... Rest>
auto foldr(First&& first, Rest&&... rest)
AUTO_RETURNS(foldr_helper<Rest...>::fold(std::forward<First>(first)(), std::forward<Rest>(rest)...));

#undef AUTO_RETURNS

} // namespace functional

#endif // CXX_UTIL_FOLD_HPP_
