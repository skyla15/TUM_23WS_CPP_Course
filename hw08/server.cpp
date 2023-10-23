#include "server.h"


net::Server::Server(uint16_t port) : serv_port{port}
{
    std::cout << "++++++++++++++++++++++++++++++++++++++++ ^^^^^^" << std::endl;
    std::cout << "+++++ SERVER CONSTRUCTOR" << std::endl;
    std::cout << "+++++ Server connected" << std::endl;
    std::cout << "+++++ Server Socket fd : " << serv_socket.fd() << std::endl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << "Server is trying to listen.. " << std::endl;
    if( setSockOpt() < 0 ){
        std::cout << "Server socket set to be reusable" << std::endl;
    }else{
        std::cout << "Server socket failed set to be reusable" << std::endl;
    }
    serv_socket.listen(serv_port);
}

net::Server::~Server(){
    std::cout << "++++++++++++++++++++++++++++++++++++++++ ^^^^^^" << std::endl;
    std::cout << "+++++ SERVER DESCTURCTOR +++++" << std::endl;
    std::cout << "+++++ Closing Server Socket" << std::endl;
    std::cout << "+++++ Server Socket fd : " << serv_socket.fd() << std::endl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++" << std::endl;
    close(serv_socket.fd());
}

net::Connection net::Server::accept() const{
    std::cout << "Server is waiting for a client to connect.. " << std::endl;
    return serv_socket.accept();
}

int net::Server::setSockOpt(){
    int iSetOption = 1;
    return setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR, (char *)&iSetOption, sizeof(iSetOption));
}

int net::Server::fd() const{
    return serv_socket.fd();
}
