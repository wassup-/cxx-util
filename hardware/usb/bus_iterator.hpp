#ifndef CXX_UTIL_HARDWARE_USB_BUS_ITERATOR_
#define CXX_UTIL_HARDWARE_USB_BUS_ITERATOR_

#include "../../filesystem/path.hpp"

#include <iterator>
#include <memory>

#include <dirent.h>

namespace hardware
{

namespace usb
{

struct bus_iterator
: std::iterator<std::input_iterator_tag, std::string>
{
public:
  bus_iterator()
  : dir_(nullptr)
  , ptr_(nullptr)
  , directory_()
  { }

  bus_iterator(const std::string &directory)
  : dir_(::opendir(directory.c_str()), &::closedir)
  , ptr_(nullptr)
  , directory_(directory)
  {
    if(dir_) {
      ptr_ = &ent_;
      increment();
    }
  }

  bus_iterator operator++(int)
  {
    bus_iterator copy = *this;
    increment();
    return copy;
  }

  bus_iterator& operator++()
  {
    increment();
    return *this;
  }

  std::string operator*() const
  {
    return directory_ + "/" + ent_.d_name;
  }

  bool operator==(const bus_iterator& other) const
  {
    return (ptr_ == other.ptr_);
  }

  bool operator!=(const bus_iterator& other) const
  {
    return !(*this == other);
  }

protected:
  void increment()
  {
    while(0 == ::readdir_r(dir_.get(), &ent_, &ptr_))
    {
      if(ptr_ == NULL) break;
      if('.' == ent_.d_name[0]) continue;
      break;
    }
  }

private:
  std::shared_ptr<DIR> dir_;
  struct dirent ent_;
  struct dirent* ptr_;
  std::string directory_;
};

} // namespace usb

} // namespace hardware

#endif // CXX_UTIL_HARDWARE_USB_BUS_ITERATOR_
