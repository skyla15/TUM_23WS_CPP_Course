#pragma once
#include "socket.h"
#include "connection.h"


namespace net {

/**
 * TCP socket server. Listens for your request to deliver you juicy data!
 */
class Server{

public:
    uint16_t serv_port;

    Server(uint16_t port);

    ~Server();

    Connection accept() const;

    int setSockOpt();

    int fd() const;

private:
    Socket serv_socket;
};

} // namespace net
