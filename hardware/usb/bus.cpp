#include "bus.hpp"

#include <dirent.h>
#include <usb.h>

namespace hardware
{

namespace usb
{

namespace
{

struct dir_manager
{
    DIR *dir;

    dir_manager(DIR *d)
    : dir(d)
    { }

    ~dir_manager() {
        closedir(dir);
    }

    operator bool() const {
        return dir;
    }
};

} // namespace

static const char* const PATHS[] = {
    "/dev/bus/usb",
    "/proc/bus/usb",
};
static std::size_t PATH_COUNT = 2;

basic_bus::basic_bus(const std::string &path, const std::string &name)
: m_path(path), m_name(name)
{ }

basic_bus::~basic_bus()
{ }

const std::string& basic_bus::name() const {
    return m_name;
}

std::string basic_bus::path() const {
    return m_path + "/" + m_name;
}

std::vector<basic_device> basic_bus::devices() const {
    std::vector<basic_device> ret;
    DIR *dir = opendir(path().c_str());
    if (dir) {
        dir_manager dm(dir);
        while (dirent *ent = readdir(dir)) {
            if ('.' == ent->d_name[0]) {
                continue;
            }
            ret.push_back(basic_device(this, ent->d_name));
        }
    }
    return ret;
}

std::vector<basic_bus> basic_bus::all() {
    return get_all_busses();
}

bool is_valid_bus_path(char const *path) {
    DIR * dir = opendir(path);
    if (!dir) {
        return false;
    }
    dir_manager dm(dir);
    while (dirent *ent = readdir(dir)) {
        if ('.' == ent->d_name[0]) {
            continue;
        }
        return true;
    }
    return false;
}

std::vector<basic_bus> get_all_busses() {
    std::vector<basic_bus> ret;
    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        char const * cur_path = PATHS[i];
        if (!is_valid_bus_path(cur_path)) {
            continue;
        }
        DIR *dir = opendir(cur_path);
        if (!dir) {
            continue;
        }
        dir_manager dm(dir);
        while (dirent *ent = readdir(dir)) {
            if ('.' == ent->d_name[0]) {
                continue;
            }
            ret.push_back(basic_bus(cur_path, ent->d_name));
        }
        break;
    }
    return ret;
}

} // namespace usb

} // namespace hardware
