#include "headers.h"

#include <iostream>     ///< cout
#include <cstring>      ///< memset
#include <errno.h>      ///< errno
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close


string_ptr getSelfEndpoint(int port = 5001, std::string const& dnsServer = googleDNSServer, int dnsPort = stdDNSPort) {
    string_ptr selfIP = getSelfIP(dnsServer, dnsPort);
    *selfIP = "tcp://" + *selfIP + ":" + std::to_string(port);
    return selfIP;
}

string_ptr getSelfIP(std::string const& dnsServer = googleDNSServer, int dnsPort = stdDNSPort) {
    socket_t sock;
    initSocket(sock);
    connectToDNSServer(sock, dnsServer, dnsPort);

    sockaddr_in name;   
    setSocketName(name, sock);

    string_ptr selfIP = std::make_shared<std::string>(
        getSelfIPWithDNSName(name)
    );
    closeSocket(sock);
    return selfIP;
}

void initSocket(socket_t& sock) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        std::cout << "Socket error" << std::endl;
    }
}

void connectToDNSServer(socket_t& sock, std::string const& dnsServer, int dnsPort) {
    sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(dnsServer.c_str());
    serv.sin_port = htons(dnsPort);

    int err = connect(sock, (sockaddr const*)&serv, sizeof(serv));
    if (err < 0) {
        std::cout << "Error number: " << errno
            << ". Error message: " << strerror(errno) << std::endl;
    }
}

void setSocketName(sockaddr_in& name, socket_t& sock) {
    socklen_t namelen = sizeof(name);
    getsockname(sock, (sockaddr*)&name, &namelen);
}

std::string getSelfIPWithDNSName(sockaddr_in const& name) {
    char buffer[80];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
    if(p == NULL) {
        std::cout << "Error number: " << errno
            << ". Error message: " << strerror(errno) << std::endl;
        exit(-1);
    }
    return p;
}

void closeSocket(socket_t& sock) {
    close(sock);
}