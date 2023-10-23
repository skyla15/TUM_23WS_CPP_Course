#include "file.h"

#include "filesystem.h"

size_t File::get_size() const { return this->content.get_size(); }

bool File::rename(std::string_view new_name) {
  auto fsptr = this->filesystem.lock();
  if (not fsptr) {
    // when filesystem vanished, or we never registered
    return false;
  }
  return fsptr->rename_file(this->name, new_name);
}

const std::string &File::get_name() const { return this->name; }

const FileContent &File::get_content() const { return this->content; }

// TODO file constructor
File::File(FileContent&& content, std::string_view name) : content{std::move(content)}, name{name} {}
