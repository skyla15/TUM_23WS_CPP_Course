#include "filesystem.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>

Filesystem::Filesystem() {}

bool Filesystem::register_file(const std::string &name,
                               const std::shared_ptr<File> &file)
{
  // TODO: Something needs to be done here
  if (name.size() == 0 or file == nullptr or this->files.contains(name))
  {
    return false;
  }

  auto thisptr = this->shared_from_this();

  // TODO: You need some checks here
  auto filefs = file->filesystem.lock();
  if (filefs != nullptr and filefs != thisptr)
  {
    return false;
  }

  file->filesystem = std::move(thisptr);

  // TODO: More updates you need to do!
  file->filesystem = this->shared_from_this();

  // TODO register a new file here
  file->name = name;
  this->files.emplace(name, file);

  return true;
}

bool Filesystem::remove_file(std::string_view name)
{
  // TODO file removal
  if (name.size() == 0)
  {
    return false;
  }

  auto it = this->files.find(name);

  if (it == std::end(this->files))
  {
    return false;
  }

  this->files.erase(it);

  return true;
}

bool Filesystem::rename_file(std::string_view source, std::string_view dest)
{
  // TODO file renaming
  if (source.size() == 0 or dest.size() == 0 or this->files.contains(dest))
  {
    return false;
  }

  auto sit = this->files.find(source);
  if (sit == std::end(this->files))
  {
    return false;
  }

  auto file = std::move(sit->second);
  file->name = dest;
  this->files.erase(sit);
  this->files.emplace(dest, std::move(file));

  return true;
}

std::shared_ptr<File> Filesystem::get_file(std::string_view name) const
{
  // TODO
  if (name.size() == 0)
  {
    return nullptr;
  }

  auto it = this->files.find(name);
  if (it == std::end(this->files))
  {
    return nullptr;
  }

  // unordered_map iterator returns (key, value)
  return it->second;
}

size_t Filesystem::get_file_count() const
{
  // TODO
  return this->files.size();
}

size_t Filesystem::in_use() const
{
  return static_cast<size_t>(
      std::accumulate(
          std::begin(this->files),
          std::end(this->files),
          0,
          [](size_t sum, auto &elem)
          {
            return sum + elem.second->get_size();
          }));
}

// convenience function so you can see what files are stored
std::string Filesystem::file_overview(bool sort_by_size)
{
  std::ostringstream output;
  // this function is not tested, but it may help you when debugging.

  output << "files in filesystem: " << std::endl;

  // for (auto&& entry : this->files) {
  //     // TODO: fix printing name, type and size
  //     output << entry.get_name << std::endl;
  // }
  return std::move(output).str();
}

std::vector<std::shared_ptr<File>>
Filesystem::files_in_size_range(size_t max, size_t min) const
{
  // TODO: find the matching files and return them
    std::vector<std::shared_ptr<File>> ret;
    for (auto&& entry : this->files) {
        size_t file_size = entry.second->get_size();
        if (min <= file_size and file_size <= max) {
            ret.push_back(entry.second);
        }
    }
    return ret;
}
