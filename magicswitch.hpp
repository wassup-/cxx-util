#ifndef CXX_UTIL_MAGICSWITCH_HPP_
#define CXX_UTIL_MAGICSWITCH_HPP_

/**
 * source: http://stackoverflow.com/a/33613901/840382
 */

#include <type_traits>
#include <utility>
#include <vector>

enum class error_code {
  no_error = 0,
  command_device_mismatch,
};

enum class command_type {
  command_jump = 1,
  number_known,
};

template<command_type t>
using command_t = std::integral_constant<command_type, t>;

using Arg = int;
using Arguments = std::vector<Arg>;


template<command_type t, class T>
error_code execute_command(command_t<t> c, const T&, const Arguments&)
{
   return error_code::command_device_mismatch;
}

template<size_t N>
struct execute_command_t
{
  template<typename T>
  error_code operator()(T&& t, const Arguments &args)
  {
    return execute_command(command_t<static_cast<command_type>(N)>{}, std::forward<T>(t), args);
  }
};

template<size_t N, template<size_t> class Executor>
struct magic_switch
{
private:
  template<typename ...Args>
  using R = std::result_of_t<Executor<0>(Args...)>;

  template<size_t I, typename ...Args>
  static R<Args...> execute_for(Args &&...args) {
    return Executor<I>{}(std::forward<Args>(args)...);
  }

  template<size_t ...Is, typename ...Args>
  R<Args...> execute(std::index_sequence<Is...>, size_t I, R<Args...> err, Args &&...args) const {
    if(I >= N) return err;
    using pf = R<Args...>(*)(Args &&...);
    static const pf table[] = {
      magic_switch::execute_for<Is, Args...>...,
      nullptr,
    };
    return table[I](std::forward<Args>(args)...);
  }

public:
  template<class ...Args>
  R<Args...> operator()(size_t I, R<Args...> err, Args &&...args) const {
    return execute(std::make_index_sequence<N>{}, I, std::forward<R<Args...>>(err), std::forward<Args>(args)...);
  }
};

template<class T>
error_code execute_magic(command_type c, T &&t, const Arguments &args) {
  using magic = magic_switch<static_cast<size_t>(command_type::number_known), execute_command_t>;
  return magic{}(static_cast<size_t>(c), error_code::command_device_mismatch, std::forward<T>(t), args);
}

template<typename Derived>
struct command_executor
{
  error_code execute_command(command_type c, const Arguments &args)
  {
    auto *self = static_cast<Derived*>(this);
    return execute_magic(c, *self, args);
  }

  error_code execute_command(command_type c, const Arguments &args) const
  {
    auto *self = static_cast<const Derived*>(this);
    return execute_magic(c, *self, args);
  }
};

#endif // CXX_UTIL_MAGICSWITCH_HPP_
