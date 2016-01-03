#ifndef CXX_UTIL_MPL_HPP_
#define CXX_UTIL_MPL_HPP_

#include <type_traits>

namespace mpl
{

template<template<typename> class Pred, typename T, typename... U>
struct find_first
{
  using type = void;
};

template<template<typename> class Pred, typename T, typename U, typename ...V>
struct find_first<Pred, T, U, V...>
{
  using type =  std::conditional_t<
                  Pred<T>::template apply<U>::value,
                  U,
                  typename find_first<Pred, T, V...>::type>;
};

}

#endif // CXX_UTIL_MPL_HPP_
