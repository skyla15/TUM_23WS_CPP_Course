#include "socket.h"



#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)


// ref : https://www.ibm.com/docs/en/i/7.4?topic=ssw_ibm_i_74/apis/gsocko.html
bool net::is_listening(int fd){
    int val;
    socklen_t len = sizeof(val);
    int result = getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &val, &len);  // 0 success, -1 false
    if(val){
        return true; // socket is listening
    }
    else{
        return false; // empty fd : -1 // else non-listening
    }
}

// Socket Constructor 
net::Socket::Socket(){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);                // open socket
    if( socket_fd == -1 ){
        handle_error("socket failed");                              // socket error check
    }else{
        int iSetOption = 1; 
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&iSetOption, sizeof(iSetOption));       
        fd_ = FileDescriptor(socket_fd);                            // register socket fd
        // std::cout << "SOCKET CONSTRUCTOR : FD => " << fd_.unwrap() << std::endl;
    }
} // ip(7) - tcp socket, returns the file descriptor

net::Socket::~Socket(){
    std::cout << "[SOCKET DESTRUCTOR] FD => " << fd_.unwrap() << std::endl;
    if(fd_.unwrap() == -1){
        return;
    }else{
        fd_.close();
        return;
    }
    std::cout << "[SOCKET DESTRUCTOR] FD => " << fd_.unwrap() << std::endl;
}


void net::Socket::listen(uint16_t port) const{
    int server_socket_fd = fd();                                    // retrieve current socket fd

    struct sockaddr_in serv_addr; // ip4 struct                     // set up a connection tool
    std::memset(&serv_addr, 0, sizeof(serv_addr));    
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(port);

    std::cout << "socket tyring to bind.." << std::endl; 
    int bindStatus = ::bind(server_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if( bindStatus < 0 ){
        handle_error("bind failed");
    }

    std::cout << "socket tyring to listen.." << std::endl;
    int listenStatus = ::listen(server_socket_fd, 5);
    if( listenStatus < 0 ){
        fd_.close();
        handle_error("socket listen failed");
    }

    std::cout << "Socket listening.. " << std::endl;
}


//https://www.tutorialspoint.com/unix_sockets/socket_core_functions.htm
net::Connection net::Socket::accept() const{
    int server_linstening_fd = fd();        // listening socket 
    int server_new_socket_fd;           // new socket to connect to a peer
    struct sockaddr_in peer_addr;       // accept stores the peer addr in peer_addr
    socklen_t peer_addr_size = sizeof(peer_addr);  // size of peer_addr

    int pid;

    std::cout << "checking if socket is listening..." << std::endl;
    bool listening = net::is_listening(server_linstening_fd);
    
    // accept one client
    if( not listening){
        throw std::runtime_error("socket not listening");
    }

    server_new_socket_fd = ::accept(server_linstening_fd, (struct sockaddr *)&peer_addr, &peer_addr_size);

    if(server_new_socket_fd < 0){
        throw std::runtime_error("accept failed");
    }

    //
    auto new_server_fd = net::FileDescriptor(server_new_socket_fd);
    return net::Connection(std::move(new_server_fd));

    // accept multiple clients 
    // if( not listening ){
    //     throw std::runtime_error("socket not bound and listening");
    // }else{
    //     while(1)
    //     {
    //         server_new_socket_fd = ::accept(server_linstening_fd, (struct sockaddr *)&peer_addr, &peer_addr_size);

    //         if(server_new_socket_fd < 0){
    //             handle_error("accept failed");
    //         }

    //         pid = fork(); // create another process

    //         if ( pid < 0 ){
    //             handle_error("fork failed");
    //         }

    //         if ( pid == 0 ){
    //             auto new_server_fd = net::FileDescriptor(server_new_socket_fd);
    //             return net::Connection(std::move(new_server_fd));
    //         }else{
    //             ::close(server_new_socket_fd);
    //         }
    //     }
    // }
}

// reference : https://www.binarytides.com/code-a-simple-socket-client-class-in-c/
net::Connection net::Socket::connect(std::string destination, uint16_t port){
    auto peer_socket_fd = fd();
    if(peer_socket_fd == -1) // if socket is already created and not moved
    {
        throw std::runtime_error("There is already a running connection");
    }

    // construct server address 
    struct hostent* host = gethostbyname(destination.c_str());  // man gethostbyname 
    struct sockaddr_in serv_addr;                      // resolving the server addr
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( port );
    serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    // connect 
    if( ::connect(peer_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    {
        handle_error("connection failed");
        // std::cout << "connection failed" << std::endl;
    }

    std::cout << "client connected to the server !" << std::endl;
    // net::FileDescriptor connection_fd = std::move(fd_);
    // return net::Connection(std::move(connection_fd));  // give the responsibility to close the connection to Connection   
    return net::Connection(std::move(fd_));
}


net::Connection net::Socket::connect(uint16_t port){
    std::cout << "ERORR POINT 1" << std::endl;
    std::cout << "Socket fd :: " << fd() << std::endl;
    auto peer_socket_fd = fd();
    if(peer_socket_fd == -1) // if socket is already created and not moved
    {
        throw std::runtime_error("There is already a running connection");
    }

    struct hostent* host = gethostbyname("localhost");
    struct sockaddr_in serv_addr;                           // sockaddr_in struct declaration
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( port );
    serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));

    std::cout << "ERORR POINT 2" << std::endl;
    std::cout << "Before connecting" << std::endl;

    if( ::connect(peer_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 )
    {
        handle_error("connection failed");
    }

    std::cout << "client connected to the server !" << std::endl;

    std::cout << "ERORR POINT 3" << std::endl;
    std::cout << "fd_ before moved : " << fd_.unwrap() << std::endl;
    net::FileDescriptor connection_fd = std::move(fd_);
    std::cout << "fd_ after moved : " << fd_.unwrap() << std::endl;
    // return net::Connection(std::move(net::FileDescriptor(connection_fd)));  // give the responsibility to close the connection to Connection   
    return net::Connection(std::move(connection_fd));
}


int net::Socket::fd() const{
    return fd_.unwrap();
}














