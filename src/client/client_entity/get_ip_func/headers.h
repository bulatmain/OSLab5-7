#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <memory>
#include <string>

std::string const googleDNSServer = "8.8.8.8";
int const stdDNSPort = 53;

using socket_t = int;

using string_ptr = std::shared_ptr<std::string>;

string_ptr getSelfEndpoint(std::string const& dnsServer, int dnsPort);

string_ptr getSelfIP(std::string const& dnsServer, int dnsPort);

void initSocket(socket_t& sock);

void connectToDNSServer(socket_t& sock, std::string const& dnsServer, int dnsPort);

void setSocketName(sockaddr_in& name, socket_t& sock);

std::string getSelfIPWithDNSName(sockaddr_in const& name);

void closeSocket(socket_t& sock);