#ifndef CXX_UTIL_SWITCH_
#define CXX_UTIL_SWITCH_

#include <utility>  // for std::forward, std::index_sequence(_for)

template<typename T, typename Fn>
struct nondefault_case
{
  T t_;
  Fn fn_;

  template<typename U>
  bool operator()(U &&u) const
  { return (std::forward<U>(u) == t_) ? (fn_(), void(), true) : false; }
};

template<typename Fn>
struct default_case
{
  Fn fn_;

  template<typename U>
  bool operator()(U &&u) const
  { return (fn_(), void(), true); }
};

template<typename ...Cases>
struct any_switch
{
  std::tuple<Cases...> cases_;

  template<typename T, typename First>
  void shortcircuit(T &&t, First &&first) const
  { std::forward<First>(first)(std::forward<T>(t)); }

  template<typename T, typename First, typename ...Rest>
  void shortcircuit(T &&t, First &&first, Rest &&...rest) const
  {
    if(std::forward<First>(first)(std::forward<T>(t))) return;
    shortcircuit(std::forward<T>(t), std::forward<Rest>(rest)...);
  }

  template<typename T, size_t ...I>
  void evaluate(T &&t, std::index_sequence<I...>) const
  { shortcircuit(std::forward<T>(t), std::get<I>(cases_)...); }

  template<typename T>
  void operator()(T &&t) const
  { evaluate(std::forward<T>(t), std::index_sequence_for<Cases...>()); }
};

template<typename T, typename ...Cases>
void switch_(T &&t, Cases &&...cases)
{
  any_switch<Cases...> stmt { { std::forward<Cases>(cases)... } };
  stmt(std::forward<T>(t));
}

template<typename T, typename Fn>
nondefault_case<T, Fn> case_(T &&t, Fn &&fn)
{ return { std::forward<T>(t), std::forward<Fn>(fn) }; }

template<typename Fn>
default_case<Fn> default_(Fn &&fn)
{ return { std::forward<Fn>(fn) }; }

#endif // CXX_UTIL_SWITCH_
