#include "directory.hpp"

#include <dirent.h>

#if !defined(_DIRENT_HAVE_D_TYPE)
  #include <sys/stat.h>
#endif

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>

namespace filesystem
{

namespace detail
{

bool filter_directories(const basic_path& directory, struct dirent* ent)
{
#if defined(_DIRENT_HAVE_D_TYPE)
  return (ent->d_type == DT_DIR);
#else
  basic_path fullpath = basic_path::join(directory, ent->d_name);
  struct stat stbuf;
  if(0 == ::stat(static_cast<std::string>(fullpath).c_str(), &stbuf))
  {
    return S_ISDIR(stbuf.st_mode);
  }
  return false;
#endif
}

bool filter_files(const basic_path& directory, struct dirent* ent)
{
#if defined(_DIRENT_HAVE_D_TYPE)
  return (ent->d_type == DT_REG);
#else
  basic_path fullpath = basic_path::join(directory, ent->d_name);
  struct stat stbuf;
  if(0 == ::stat(static_cast<std::string>(fullpath).c_str(), &stbuf))
  {
    return S_ISREG(stbuf.st_mode);
  }
  return false;
#endif
}

} // namespace detail

template<bool(*Filter)(const basic_path&, struct dirent*)>
struct basic_directory_iterator
: std::iterator<std::input_iterator_tag, basic_path>
{
public:
  basic_directory_iterator()
  : dir_(NULL)
  , ptr_(NULL)
  , directory_(basic_path::invalid_path)
  { }

  basic_directory_iterator(const basic_path &directory)
  : dir_(::opendir(static_cast<std::string>(directory).c_str()), &::closedir)
  , ptr_(&ent_)
  , directory_(directory)
  {
    increment();
  }

  basic_directory_iterator operator++(int)
  {
    basic_directory_iterator copy = *this;
    increment();
    return copy;
  }

  basic_directory_iterator& operator++()
  {
    increment();
    return *this;
  }

  basic_path operator*() const
  {
    return basic_path::join(directory_, ent_.d_name);
  }

  bool operator==(const basic_directory_iterator& other) const
  {
    return (ptr_ == other.ptr_);
  }

  bool operator!=(const basic_directory_iterator& other) const
  {
    return !(*this == other);
  }

protected:
  void increment()
  {
    while(0 == ::readdir_r(dir_.get(), &ent_, &ptr_))
    {
      if(ptr_ == NULL) break;
      if(Filter(directory_, &ent_)) break;
    }
  }

private:
  std::shared_ptr<DIR> dir_;
  struct dirent ent_;
  struct dirent* ptr_;
  const basic_path& directory_;
};

basic_directory::directories_t basic_directory::directories() const
{
  using input_iterator = basic_directory_iterator<detail::filter_directories>;
  return { input_iterator(path_), input_iterator() };
}

basic_directory::files_t basic_directory::files() const
{
  using input_iterator = basic_directory_iterator<detail::filter_files>;
  return { input_iterator(path_), input_iterator() };
}

} // namespace filesystem
