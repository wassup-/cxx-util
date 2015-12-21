#ifndef FILESYSTEM_EXCEPTIONS_HPP_
#define FILESYSTEM_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

namespace filesystem
{

struct exception : std::runtime_error
{
  using std::runtime_error::runtime_error;
};

struct access_exception : exception
{
  using exception::exception;
};

struct file_not_found_exception : exception
{
  file_not_found_exception(const std::string &path)
  : exception("File " + path + " not found")
  { }
};

struct directory_not_found_exception : exception
{
  directory_not_found_exception(const std::string &path)
  : exception("Directory " + path + " not found")
  { }
};

} // namespace filesystem

#endif
