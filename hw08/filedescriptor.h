#pragma once

#include <optional>
#include <utility>

#include <unistd.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>

namespace net {

/// Wrapper for a Linux style file descriptor. It represents a potentially empty file descriptor.
/// The wrapper represents a unique ownership model, i.e. the file descriptor will get invalided
/// with the end of the lifetime of the object.
///
/// The following code should help you understand the ownership model:
/// ```cpp
/// auto fdraw {get_socket_fd_from_somewhere()};
/// {
///     FileDescriptor fd{fdraw};
///
///     // use fd, do whatever with it
/// }
/// // fd should not be usable anymore
/// ```
class FileDescriptor {
public:
    /// Default constructor for empty file descriptor
    FileDescriptor() = default;

    /// Construct from a integer file descriptor returned from the C API, take ownership of the
    /// descriptor
    explicit FileDescriptor(int fd);

    /// Closef the file descriptor (if present and valid)
    /// Check out: close(3)
    ~FileDescriptor();

    // TODO: Implement both copy and move constructors and assignment 
    //       for the ownership model described in the class description.

    FileDescriptor(const FileDescriptor& other) = delete; // copy constructor
    FileDescriptor& operator=(const FileDescriptor& other) = delete; // copy assignment

    FileDescriptor(FileDescriptor&& other)              noexcept; // move constructor  
    FileDescriptor& operator=(FileDescriptor&& other)   noexcept; // move assignment

    void swap(FileDescriptor& other)                    noexcept;

    void close() const;
    
    /// Return the underlying file descriptor, 
    /// if not present return -1 (this is quite standard for
    /// linux systems)
    int unwrap() const;

private:
    std::optional<int> fd_ {};
};
} // namespace net
    