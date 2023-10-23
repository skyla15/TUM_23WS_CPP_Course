#include "document.h"


Document::Document(FileContent &&content) : File{std::move(content)} {}

// TODO provide file type
std::string_view Document::get_type() const { return "DOC"; }

size_t Document::get_raw_size() const {
  // TODO get the document size
  return this->content.get_size();
}

size_t Document::get_size() const {
  return this->content.get_size();
}


unsigned Document::get_character_count() const {
  // TODO count non whitespace characters
  unsigned cnt{0};
  // std::string_view data = *(this->content.get());
  // for(auto& c : data){
  //   if( (c != ' ') && (c != '\n') && (c != '\t') ) cnt++;
  // }
  auto data = this->content.get().get();
  for (auto&& character : *data) {
    switch (character) {
      case ' ':
      case '\n':
      case '\t':
      break;
      default:
      cnt += 1;
} }
  
  return cnt;
}

// TODO content update function
void Document::update(FileContent &&new_content){
  this->content = std::move(new_content);
}
