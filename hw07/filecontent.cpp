#include "filecontent.h"


FileContent::FileContent(const std::string &content) : fileContent{std::make_shared<const std::string>(content)} {}

FileContent::FileContent(std::string&& content) : fileContent{std::make_shared<std::string>(std::move(content))} {}

FileContent::FileContent(const char *content) : fileContent{std::make_shared<const std::string>(content)} {}

    /** what's the actual storage size of the file content? */
size_t FileContent::get_size() const{
    return this->fileContent->size();
}

    /** get a read-only handle to the data */
std::shared_ptr<const std::string> FileContent::get() const{
    return fileContent;
}

    // add automatic comparisons
bool FileContent::operator==(const FileContent& other) const noexcept{
    return fileContent == other.fileContent;
}
