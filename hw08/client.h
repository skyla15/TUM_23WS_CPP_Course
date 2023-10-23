#pragma once

#include "connection.h"
#include "socket.h"

namespace net {

    #define handle_error(msg) \
    do { perror(msg + '/n'); exit(EXIT_FAILURE); } while (0)

/**
 * TCP socket client. Can connect to a server given a destination IP address and a port.
 */
class Client{

public:

    uint16_t clnt_port;

    Client();

    ~Client();

    Connection connect(std::string destination, uint16_t port);

    Connection connect(uint16_t port);


private:
    Socket clnt_socket;
};

} // namespace net
