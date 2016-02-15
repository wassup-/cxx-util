#ifndef CXX_UTIL_SWITCH_
#define CXX_UTIL_SWITCH_

#include <tuple>        // for std::tuple, std::get
#include <type_traits>  // for std::enable_if, std::(true|false)_type, std::result_of
#include <utility>      // for std::forward, std::index_sequence(_for)

namespace detail
{

template<typename T, T...>
struct integer_sequence { };

template<size_t ...I>
using index_sequence = integer_sequence<size_t, I...>;

template<size_t N, size_t ...I>
struct make_index_sequence : make_index_sequence<(N - 1), (N - 1), I...> { };
template<size_t ...I>
struct make_index_sequence<0, I...> : index_sequence<I...> { };

template<typename ...T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;

} // namespace

struct switch_continuation
{
  bool continue_;

  bool operator()() const { return continue_; };
};

static switch_continuation const break_ { false };
static switch_continuation const continue_ { true };

template<typename>
struct is_continuation : std::false_type { };
template<>
struct is_continuation<switch_continuation> : std::true_type { };

template<typename T>
struct is_continuation<const T> : is_continuation<T> { };
template<typename T>
struct is_continuation<T&> : is_continuation<T> { };

template<typename Fn>
switch_continuation continuation(Fn &&fn, typename std::enable_if<is_continuation<typename std::result_of<Fn()>::type>::value, void*>::type = nullptr)
{ return fn(); }

template<typename Fn>
switch_continuation continuation(Fn &&fn, typename std::enable_if<!is_continuation<typename std::result_of<Fn()>::type>::value, void*>::type = nullptr)
{ return (fn(), continue_); }

template<typename T, typename Fn>
struct nondefault_case
{
  T t_;
  Fn fn_;

  template<typename U>
  bool evaluate(U &&u) const
  { return std::forward<U>(u) == t_; }

  switch_continuation operator()() const
  { return continuation(fn_); }
};

template<typename Fn>
struct default_case
{
  Fn fn_;

  template<typename U>
  bool evaluate(U &&u) const
  { return true; }

  switch_continuation operator()() const
  { return continuation(fn_); }
};

template<typename ...Cases>
struct any_switch
{
  std::tuple<Cases...> cases_;

  template<typename Matched, typename T>
  static void shortcircuit(Matched matched, T &&t)
  { }

  template<typename Eval, typename T, typename First, typename ...Rest>
  static void shortcircuit(Eval eval, T &&t, First &&first, Rest &&...rest)
  {
    if(!eval || std::forward<First>(first).evaluate(std::forward<T>(t))) {
      switch_continuation &&continuation = std::forward<First>(first)();
      if(!continuation()) return;
      else shortcircuit(std::false_type{ }, std::forward<T>(t), std::forward<Rest>(rest)...);
    } else {
      shortcircuit(eval, std::forward<T>(t), std::forward<Rest>(rest)...);
    }
  }

  template<typename T, size_t ...I>
  void evaluate(T &&t, detail::index_sequence<I...>) const
  { shortcircuit(std::true_type{ }, std::forward<T>(t), std::get<I>(cases_)...); }

  template<typename T>
  void operator()(T &&t) const
  { evaluate(std::forward<T>(t), detail::index_sequence_for<Cases...>()); }
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
