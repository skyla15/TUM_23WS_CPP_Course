#include "audio.h"

Audio::Audio(FileContent &&content, unsigned duration)
    : File{std::move(content)}, duration{duration} {}

// TODO provide file type
std::string_view Audio::get_type() const { return "AUD"; }

size_t Audio::get_raw_size() const {
    // TODO calculate raw size
    return 2 * 48000 * this->duration * 2;
}

size_t Audio::get_size() const {
  return this->content.get_size();
}

unsigned Audio::get_duration() { return this->duration; }

// TODO content update function
void Audio::update(FileContent&& new_content, unsigned new_duration) {
  this->content = std::move(new_content);
  this->duration = new_duration;
}

