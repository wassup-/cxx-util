#ifndef FILESYSTEM_EXCEPTIONS_HPP_
#define FILESYSTEM_EXCEPTIONS_HPP_

#include <stdexcept>

namespace filesystem
{

struct filesystem_exception : std::runtime_error
{
  using std::runtime_error::runtime_error;
};

struct file_not_found_exception : filesystem_exception
{
  file_not_found_exception(const std::string& filename)
  : filesystem_exception("File " + filename + " not found")
  { }
};

} // namespace filesystem

#endif
