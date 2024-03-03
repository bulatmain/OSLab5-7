#ifndef GET_IP_FUNC_HPP
#define GET_IP_FUNC_HPP

#include <iostream>     ///< cout
#include <cstring>      ///< memset
#include <errno.h>      ///< errno
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close
#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <string>

namespace lab5_7 {
    static std::string const googleDNSServer = "8.8.8.8";
    static uint16_t const stdDNSPort = 53;

    class SelfIPInfo {
    public:
        static int const minGeneratedPort = 6000;
        static int const maxGeneratedPort = 7000;

        using socket_t = int;

        static std::string generateSelfEndpoint() {
            auto port = generatePort();
            return getSelfEndpoint(port);
        }

        static std::string getSelfEndpoint(int port = 5001, std::string const& dnsServer = googleDNSServer, uint16_t dnsPort = stdDNSPort) {
            std::string selfIP = getSelfIP(dnsServer, dnsPort);
            return "tcp://" + selfIP + ":" + std::to_string(port);
        }

        static std::string getSelfIP(std::string const& dnsServer = googleDNSServer, uint16_t dnsPort = stdDNSPort) {
            socket_t sock;
            initSocket(sock);
            connectSocketToDNSServer(sock, dnsServer, dnsPort);

            sockaddr_in name;   
            setSocketName(name, sock);

            std::string selfIP = getSelfIPWithDNSName(name);
            closeSocket(sock);

            return selfIP;
        }

    protected:
        SelfIPInfo() = default;

        static int generatePort() {
            srand((unsigned)time(NULL));
            int portRange = maxGeneratedPort - minGeneratedPort + 1;
            int port = minGeneratedPort + (rand() + 1) % portRange;
            return port;
        }

        inline static void printErrorMessage() { \
            std::cout << "Error number: " << errno \
                << ". Error message: " << strerror(errno) << std::endl; \
        }

        static void initSocket(socket_t& sock) {
            sock = socket(AF_INET, SOCK_DGRAM, 0);
            if(sock < 0) {
                std::cout << "Socket error" << std::endl;
            }
        }

        static void connectSocketToDNSServer(socket_t& sock, std::string const& dnsServer, uint16_t dnsPort) {
            sockaddr_in serv;
            memset(&serv, 0, sizeof(serv));
            serv.sin_family = AF_INET;
            serv.sin_addr.s_addr = inet_addr(dnsServer.c_str());
            serv.sin_port = htons(dnsPort);

            int err = connect(sock, (sockaddr const*)&serv, sizeof(serv));
            if (err < 0) {
                printErrorMessage();
            }
        }

        static void setSocketName(sockaddr_in& name, socket_t& sock) {
            socklen_t namelen = sizeof(name);
            getsockname(sock, (sockaddr*)&name, &namelen);
        }

        static std::string getSelfIPWithDNSName(sockaddr_in const& name) {
            char buffer[80];
            const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
            if(p == NULL) {
                printErrorMessage();
                exit(-1);
            }
            return p;
        } 

        static void closeSocket(socket_t const& sock) {
            close(sock);
        }


    };
};








#endif


