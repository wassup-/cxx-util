#ifndef FILESYSTEM_FILE_HPP_
#define FILESYSTEM_FILE_HPP_

#include "path.hpp"

#include <string>
#include <vector>

namespace filesystem
{

class basic_file
{
public:
  basic_file(std::string filename) : path_(filename) { }

  const basic_path& path() const { return path_; }

  unsigned long size() const;
  const std::vector<unsigned char>& data() const;

  bool load();
  bool save(long chunk_size) const;

  template<typename Access>
  bool has(Access);

private:
  basic_path path_;
  std::vector<unsigned char> data_;
};

} // namespace filesystem

#endif
