#include "client.h"

net::Client::Client(){
    std::cout << "----------------------------------------- ^^^^^^" << std::endl;
    std::cout << "----- CLIENT CONSTRUCTOR -----" << std::endl;
    std::cout << "[Client] Created " << std::endl;
    // std::cout << "----- CLIENT CONSTRUCTOR -----" << std::endl;
    // std::cout << "Client connected" << std::endl;
    std::cout << "[Client] Socket fd : " << clnt_socket.fd() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

net::Client::~Client(){
    std::cout << "----------------------------------------- ^^^^^^" << std::endl;
    std::cout << "----- CLIENT DESCTRUCTOR -----" << std::endl;
    // std::cout << "Closing Client Socket" << std::endl;
    std::cout << "Client Socket fd : " << clnt_socket.fd() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    close(clnt_socket.fd());
}

net::Connection net::Client::connect(std::string destination, uint16_t port)
{
    std::cout << "[Client] connecting to server.. " << std::endl;
    return clnt_socket.connect(destination, port);
}

net::Connection net::Client::connect(uint16_t port)
{
    std::cout << "[Client] connecting to server.. " << std::endl;
    return clnt_socket.connect(port);
}