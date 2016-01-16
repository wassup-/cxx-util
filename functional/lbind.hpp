#ifndef CXX_UTIL_LBIND_HPP_
#define CXX_UTIL_LBIND_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

namespace meta
{

template<typename T>
using type_t = typename T::type;

template<typename T, typename U>
using apply_t = type_t<typename U::template apply<T> >;

template<template<typename> class T>
struct quote
{
  template<typename U>
  struct apply { using type = type_t<T<U>>; };
};

template<typename... T>
struct compose
{
private:
  template<typename U, typename F, typename... B>
  struct compose_impl
  {
    using inner = type_t<compose_impl<U, B...>>;
    using type = apply_t<inner, F>;
  };

  template<typename U, typename F>
  struct compose_impl<U, F>
  { using type = apply_t<U ,F>; };

public:
  template<typename U>
  struct apply
  { using type = type_t<compose_impl<U, T...>>; };
};

template<typename C, typename... T>
struct count_if : std::integral_constant<int, (apply_t<T, C>::value + ...)> { };

} // namespace meta

namespace functional
{

namespace detail
{

template<int>
struct placeholder { };

template<typename T>
struct argument { T value; };

template<typename>
struct is_placeholder : std::false_type { };
template<int I>
struct is_placeholder<placeholder<I> > : std::true_type { };

template<
  typename T,
  typename U = meta::apply_t<
                 T,
                 meta::compose<
                   meta::quote<std::remove_cv>,
                   meta::quote<std::remove_reference> > > >
argument<U> wrap(T &&arg) { return { std::forward<T>(arg) }; }

template<int I>
placeholder<I> wrap(placeholder<I>) { return { }; }

template<typename T, typename... Args>
auto unwrap(argument<T> &&arg, Args &&...)
{ return arg.value; }

template<int I, typename... Args>
auto unwrap(placeholder<I>, Args &&...args)
{ return std::get<(I - 1)>(std::tie(std::forward<Args>(args)...)); }

struct invalid_number_of_arguments
{
  template<int Have>
  struct have
  {
    template<int Need>
    struct need
    {
      static_assert((Have == Need), "Invalid number of arguments");
    };
  };
};

template<int Have, int Need>
using check_argument_count = typename invalid_number_of_arguments::template have<Have>::template need<Need>;

} // namespace detail

namespace placeholders
{

constexpr static auto _1 = detail::placeholder<1>{};
constexpr static auto _2 = detail::placeholder<2>{};
constexpr static auto _3 = detail::placeholder<3>{};
constexpr static auto _4 = detail::placeholder<4>{};
constexpr static auto _5 = detail::placeholder<5>{};
constexpr static auto _6 = detail::placeholder<6>{};
constexpr static auto _7 = detail::placeholder<7>{};
constexpr static auto _8 = detail::placeholder<8>{};
constexpr static auto _9 = detail::placeholder<9>{};

} // namespace placeholders

template<typename Fn, typename... Args>
auto bind(Fn &&fn, Args &&...args)
{
  using positionals = meta::count_if<
                        meta::compose<
                          meta::quote<detail::is_placeholder>,
                          meta::compose<
                            meta::quote<std::remove_cv>,
                            meta::quote<std::remove_reference> > >,
                        Args...>;

  return [=](auto... outer) {
    (void)detail::check_argument_count<sizeof...(outer), positionals::value>{ };
    return fn(detail::unwrap(detail::wrap(args), outer...)...);
  };
}

} // namespace functional

#endif // CXX_UTIL_LBIND_HPP_
