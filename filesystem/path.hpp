#ifndef FILESYSTEM_PATH_HPP_
#define FILESYSTEM_PATH_HPP_

#include <string>

namespace filesystem
{

class basic_path
{
public:
  constexpr static char separator { '/' };
  const static basic_path invalid_path;

public:
  basic_path()
  : path_()
  { }

  basic_path(std::string p)
  : path_(p)
  { }

  bool is_absolute() const;
  bool is_relative() const;

  operator std::string() const;

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
