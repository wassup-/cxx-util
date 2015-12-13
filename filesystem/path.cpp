#include "path.hpp"

#include <algorithm>
#include <iterator>

namespace filesystem
{

constexpr char basic_path::separator;

namespace detail
{

std::string joinpaths(const std::string& left, const std::string& right, char separator)
{
  const bool left_slash = (left.back() == separator);
  const bool right_slash = (right.front() == separator);
  if(!(left_slash || right_slash))
    return left + "/" + right;
  return left + right;
}

} // namespace detail

bool basic_path::is_absolute() const
{
  if(path_.size() < 1) return false;
  return (separator == path_.front());
}

bool basic_path::is_relative() const
{
  if(path_.size() < 1) return false;
  return (separator != path_.front());
}

std::string basic_path::directory() const
{
  auto it = std::find(path_.rbegin(),
                      path_.rend(),
                      separator);
  if(it == path_.rend()) it = path_.rbegin();
  return { begin(path_), it.base() };
}

std::string basic_path::filename() const
{
  auto it = std::find(path_.rbegin(),
                      path_.rend(),
                      separator);
  return { it.base(), end(path_) };
}

std::string basic_path::extension() const
{
  auto it = std::find(path_.rbegin(),
                      path_.rend(),
                      '.');
  return { it.base(), end(path_) };
}

basic_path::operator std::string() const
{
  return path_;
}

basic_path basic_path::join(const basic_path& l, const basic_path& r)
{
  return { detail::joinpaths(l.path_, r.path_, separator) };
}

basic_path basic_path::join(const basic_path& l, std::string r)
{
  return { detail::joinpaths(l.path_, r, separator) };
}

} // namespace filesystem
