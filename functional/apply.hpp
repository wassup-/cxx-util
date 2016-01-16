#ifndef CXX_UTIL_FUNCTIONAL_APPLY_HPP_
#define CXX_UTIL_FUNCTIONAL_APPLY_HPP_

#include <tuple>
#include <utility>

namespace functional
{

namespace detail
{

struct tuplelike_tag { };
struct arraylike_tag { };

template<typename>
struct apply_traits;

template<typename... Ts>
struct apply_traits<std::tuple<Ts...> >
{
  using tag = tuplelike_tag;
  constexpr static std::size_t size = sizeof...(Ts);
};

template<typename T, typename U>
struct apply_traits<std::pair<T, U> >
{
  using tag = tuplelike_tag;
  constexpr static std::size_t size = 2;
};

template<typename T, std::size_t Sz>
struct apply_traits<std::array<T, Sz> >
{
  using tag = arraylike_tag;
  constexpr static std::size_t size = Sz;
};

template<typename T, std::size_t Sz>
struct apply_traits<T[Sz]>
{
  using tag = arraylike_tag;
  constexpr static std::size_t size = Sz;
};

template<typename Functor, size_t ...Indices, typename Arguments>
auto apply(tuplelike_tag, std::index_sequence<Indices...>, Functor &&fn, Arguments &&args)
{
  using std::get;
  return std::forward<Functor>(fn)(get<Indices>(args));
}

template<typename Functor, size_t ...Indices, typename Arguments>
auto apply(arraylike_tag, std::index_sequence<Indices...>, Functor &&fn, Arguments &&args)
{
  using std::get;
  return std::forward<Functor>(fn)(get<Indices>(args));
}

} // namespace detail

template<typename Functor, typename Arguments>
auto apply(Functor &&fn, Arguments &&args)
{
  using traits = detail::apply_traits<Arguments>;
  using tag = typename traits::tag;
  using indices = std::make_index_sequence<traits::size>;
  return detail::apply(tag{}, indices{}, std::forward<Functor>(fn), std::forward<Arguments>(args));
}

} // namespace functional

#endif // CXX_UTIL_FUNCTIONAL_APPLY_HPP_
