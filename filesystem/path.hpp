#ifndef FILESYSTEM_PATH_HPP_
#define FILESYSTEM_PATH_HPP_

#include "../config.hpp"

#include <string>

namespace filesystem
{

class basic_path
{
public:
#ifdef CXXU_SYSTEM_WINDOWS
  constexpr static char separator { '\\' };
#else
  constexpr static char separator { '/' };
#endif
  const static basic_path invalid_path;

public:
  basic_path()
  : path_()
  { }

  explicit basic_path(std::string p)
  : path_(p)
  { }

  bool is_absolute() const;
  bool is_relative() const;

  explicit operator std::string() const;

  std::string directory() const;
  std::string extension() const;
  std::string filename() const;

public:
  static basic_path join(const basic_path&, const basic_path&);
  static basic_path join(const basic_path&, std::string);

private:
  std::string path_;
};

} // namespace filesystem

#endif
