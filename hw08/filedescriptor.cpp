#include "filedescriptor.h"

// reference page : https://android.googlesource.com/platform/bionic/+/master/docs/fdsan.md#Enabling-fdsan-as-a-C_library-implementer
// unwrap here in the program is similar to release 

// https://github.com/Loki-Astari/Examples/tree/master/Version2


#define handle_error(msg) \
    do { perror(msg + '/n'); exit(EXIT_FAILURE); } while (0)

// Constructor
net::FileDescriptor::FileDescriptor(int fd) : fd_{fd} {
    if( fd_.value() == -1 ){
        handle_error("File Descriptor not constructed");
    }
} 

// Desctructor
net::FileDescriptor::~FileDescriptor(){
    if( fd_.value_or(-1) == -1 ){
        return; // this object has been closed or moved;
    }

    close();
}

// move constructor
net::FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept
{
   *this = std::move(other); 
} 

// move assignment
net::FileDescriptor& net::FileDescriptor::operator=(FileDescriptor&& other) noexcept
{
    if(this == &other){
        return *this;
    }
    if(other.fd_.has_value()){
        fd_ = std::move(other.fd_);
        other.fd_ = -1;
    }else{
        fd_ = -1;
    }
    return *this;
}

// close
void net::FileDescriptor::close() const
{
    ::close(fd_.value());
}


int net::FileDescriptor::unwrap() const{
    // return fd_.value_or(-1);
    // std::cout << fd_.value_or(-111) << std::endl;
    if(fd_.has_value() )
    {
        return fd_.value();
    }
    return -1;
}