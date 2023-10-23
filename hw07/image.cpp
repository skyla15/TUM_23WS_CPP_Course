#include "image.h"

Image::Image(FileContent &&content, resolution_t res)
    : File{std::move(content)}, resolution{res} {}

// TODO provide file type
std::string_view Image::get_type() const { return "IMG"; }

size_t Image::get_raw_size() const {
  // TODO calculate raw size
  return 4 * this->resolution[0] * this->resolution[1];
  return 0;
}

size_t Image::get_size() const {
  return this->content.get_size();
}

auto Image::get_resolution() const -> resolution_t { return this->resolution; }

// TODO content update function
void Image::update(FileContent&& new_content, resolution_t size) {
  this->content = std::move(new_content);
  this->resolution = size;
}