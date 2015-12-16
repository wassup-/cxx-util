#ifndef FILESYSTEM_DIRECTORY_HPP_
#define FILESYSTEM_DIRECTORY_HPP_

#include "path.hpp"

#include <vector>

namespace filesystem
{

class basic_directory
{
public:
  using directories_t = std::vector<basic_path>;
  using files_t = std::vector<basic_path>;

public:
  basic_directory(basic_path filename) : path_(filename) { }

  const basic_path& path() const { return path_; }

  directories_t directories() const;
  files_t files() const;

private:
  basic_path path_;
};

} // namespace filesystem

#endif
