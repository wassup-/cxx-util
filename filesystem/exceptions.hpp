#ifndef FILESYSTEM_EXCEPTIONS_HPP_
#define FILESYSTEM_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

namespace filesystem
{

struct filesystem_exception : std::runtime_error
{
  using std::runtime_error::runtime_error;
};

struct filesystem_access_exception : filesystem_exception
{
  using filesystem_exception::filesystem_exception;
};

struct file_not_found_exception : filesystem_exception
{
  file_not_found_exception(const std::string &path)
  : filesystem_exception("File " + path + " not found")
  { }
};

struct directory_not_found_exception : filesystem_exception
{
  directory_not_found_exception(const std::string &path)
  : filesystem_exception("Directory " + path + " not found")
  { }
};

} // namespace filesystem

#endif
