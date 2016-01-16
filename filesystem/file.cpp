#include "file.hpp"
#include "directory.hpp"

#include "../math/divroundup.hpp"

#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>

namespace filesystem
{

namespace detail
{

std::string calc_checksum(const std::string& str)
{
  // TODO
  return str;
}

template<typename InputIterator, typename OutputIterator, typename Size>
InputIterator copy_n_safe(InputIterator begin,
                          InputIterator end,
                          Size count,
                          OutputIterator out)
{
  for(; (begin != end) && (count > 0); ++begin, --count)
  {
    *out++ = *begin;
  }
  return begin;
}

} // namespace detail

unsigned long basic_file::size() const
{
  struct stat stbuf;
  if(0 == ::stat(static_cast<std::string>(path_).c_str(), &stbuf))
  {
    return stbuf.st_size;
  }
  return -1;
}

const std::vector<unsigned char>& basic_file::data() const
{
  return data_;
}

bool basic_file::load()
{
  const std::string checksum = detail::calc_checksum(path_.filename());
  std::vector<std::string> chunk_files;
  for(const auto& filename : basic_directory{ path_.directory() }.files())
  {
    if(filename.size() < checksum.size()) continue;
    if(!std::equal(begin(checksum),
                   end(checksum),
                   begin(filename))) continue;
    chunk_files.push_back(filename);
  }
  std::sort(begin(chunk_files), end(chunk_files));
  for(const auto& filename : chunk_files)
  {
    std::ifstream is { filename, std::ios::binary };
    if(!is.is_open()) return false;
    std::copy(std::istream_iterator<unsigned char>(is),
              std::istream_iterator<unsigned char>(),
              std::back_inserter(data_));
  }
  return true;
}

bool basic_file::save(long chunk_size) const
{
  const std::string checksum = detail::calc_checksum(path_.filename());
  const long chunk_count = divroundup(size(), chunk_size);
  for(long i = 0; i < chunk_count; ++i) {
    const std::string filename = checksum + "_" + std::to_string(i);
    std::ofstream os { filename, std::ios::binary };
    if(!os.is_open()) return false;
    detail::copy_n_safe(next(begin(data_), (i * chunk_size)),
                        end(data_),
                        chunk_size,
                        std::ostream_iterator<unsigned char>(os));
  }
  return true;
}

} // namespace filesystem
