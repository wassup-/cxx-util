#ifndef CXX_UTIL_STATIC_MAP_HPP_
#define CXX_UTIL_STATIC_MAP_HPP_

#include <tuple>
#include <type_traits>

template<typename T>
struct identity { using type = T; };

template<typename T>
using invoke = typename T::type;

template<template<typename...> class C, typename ...T>
using concrete = invoke<C<T...>>;

template<typename K,
         typename V,
         template<typename> class transform = identity>
struct static_kv
{
  using key = transform<K>;
  using value = transform<V>;

  using inverse = static_kv<V, K, transform>;

  using storage = std::pair<invoke<key>, invoke<value> >;
};

namespace detail
{

template<bool B>
using bool_ = std::integral_constant<bool, B>;

template<int I>
using int_ = std::integral_constant<int, I>;

template<typename ...>
struct and_ : bool_<true> { };

template<typename T>
struct and_<T> : bool_<!!T::value> { };

template<typename T, typename ...U>
struct and_<T, U...> : bool_<(T::value && and_<U...>::value)> { };


template<int Idx, typename T, typename ...Ts>
struct index_of_impl : int_<-1> { };

template<int Idx, typename T, typename ...Ts>
struct index_of_impl<Idx, T, T, Ts...> : int_<Idx> { };

template<int Idx, typename T, typename U, typename ...Ts>
struct index_of_impl<Idx, T, U, Ts...> : index_of_impl<(Idx + 1), T, Ts...> { };

template<typename T, typename ...Ts>
struct index_of : index_of_impl<0, T, Ts...>::type { };


template<typename T, typename ...Ts>
struct contained_in : bool_<(index_of<T, Ts...>::value >= 0)> { };


template<int Idx, typename... T>
struct type_at;

template<int Idx, typename T, typename ...U>
struct type_at<Idx, T, U...> : type_at<(Idx - 1), U...> { };

template<typename T, typename ...U>
struct type_at<0, T, U...> : identity<T> { };


template<typename Key, typename... KV>
struct type_for : type_at<
                    index_of<Key, invoke<typename KV::key>...>::value,
                    invoke<typename KV::value>...> { };


template<typename ...T>
struct is_unique : bool_<true> { };

template<typename T, typename U>
struct is_unique<T, U> : bool_<!std::is_same<T, U>::value> { };

template<typename T, typename U, typename ...V>
struct is_unique<T, U, V...> : and_<
                                 is_unique<T, U>,
                                 is_unique<T, V...>,
                                 is_unique<U, V...> > { };

} // namespace detail

template<typename ...KV>
struct static_map
{
  static_assert(detail::is_unique<invoke<typename KV::key>...>{}, "Keys must be unique");

  using inverse = static_map<typename KV::inverse...>;

  using storage = std::tuple<typename KV::storage...>;

  constexpr static size_t size = sizeof...(KV);

  template<typename Key>
  using contains = invoke<detail::contained_in<Key, invoke<typename KV::key>...> >;

  template<int Idx>
  using key_at = invoke<detail::type_at<Idx, invoke<typename KV::key>...> >;

  template<int Idx>
  using value_at = invoke<detail::type_at<Idx, invoke<typename KV::value>...> >;

  template<typename Key>
  using value_for = invoke<detail::type_for<Key, KV...> >;

  template<template<typename> class visitor>
  struct apply_visitor : visitor<static_map>::template visit<invoke<typename KV::key>, invoke<typename KV::value> >... { };
};

#endif // CXX_UTIL_STATIC_MAP_HPP_
