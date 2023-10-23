#pragma once

#include "connection.h"



#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

[[nodiscard]] ssize_t net::send(int fd, std::span<const char> data){
    std::cout << " sending data... " << std::endl;
    ssize_t bytes_sent = ::send(fd, data.data(), data.size(), 0);
    return bytes_sent;
}

[[nodiscard]] ssize_t net::receive(int fd, std::span<char> buf){
    std::cout << " receiving data... " << std::endl;
    ssize_t bytes_received = ::recv(fd, buf.data(), buf.size(), 0);
    return bytes_received;
}

net::Connection::Connection(FileDescriptor&& fd){
    if(this->fd_.unwrap() != fd.unwrap())
        fd_ = std::move(fd);
    std::cout << "[CONNECTION] FD -> : " << fd_.unwrap() << std::endl;
}


net::Connection::~Connection(){
    std::cout << "[CONNECTION DESCTRUCTOR] :: FD -> " << fd_.unwrap() << std::endl;
    fd_.close();
}


void net::Connection::send(std::string_view data) const{
    int send_status = net::send(fd(), std::span<const char>(data)); 
    if(send_status == -1) handle_error("send failed");
}


void net::Connection::send(std::istream& data) const{
    std::string str((std::istreambuf_iterator<char>(data)), std::istreambuf_iterator<char>());
    int send_status = net::send(fd(), std::span<const char>(str));
    if( send_status == -1) handle_error("send failed");
}

// slide 2022-11-22 -> stream 
[[nodiscard]] ssize_t net::Connection::receive(std::ostream& stream) const{
    std::cout << "[RECEIVE FUNCTION]" << std::endl;
    std::cout << "[RECEIVE FUNCTION -- FD ] : " << fd() << std::endl;
    constexpr size_t buffer_size{128};
    ssize_t bytes_received = 0;
    int cur_sock_fd = fd();
    std::vector<char> buffer(buffer_size);
    std::span<char> buf(buffer);

    bytes_received = net::receive(cur_sock_fd, buf);

    if( bytes_received  < 0 ){
        handle_error("receive failed");
    }

    stream.write(buf.data(), bytes_received);

    return bytes_received;
}


// https://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
[[maybe_unused]] ssize_t net::Connection::receive_all(std::ostream& stream) const{
    int cur_sock_fd = fd();
    std::cout << "[RECEIVE ALL FUNCTION]" << std::endl;
    std::cout << "[RECEIVE ALL FUNCTION -- FD ] : " << fd() << std::endl;
    ssize_t bytes_received = 0;
    ssize_t total_bytes_received = 0;
    
    while(true){
        ssize_t bytes_received = receive(stream);
        if(bytes_received < 0){
            handle_error("receive faile");
        }

        if(bytes_received == 0){
            // no more data available to read;
            break;
        }

        total_bytes_received += bytes_received;
    }    
    std::cout << "total bytes received :: " << total_bytes_received << std::endl;
    
    return total_bytes_received;
}

int net::Connection::fd() const{
    return fd_.unwrap();
}

    


// multiple sockets 
// https://stackoverflow.com/questions/13479760/c-socket-recv-and-send-all-data