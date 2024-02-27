#include <iostream>
#include <sstream>

#include <infrastructure/message/message_module.hpp>
#include <infrastructure/connection_wrapper/connection_module.hpp>

using namespace lab5_7;

#include <chrono>

int main(int argc, char** argv) {  
    std::string endpoint(argv[1]);
    PullQueue pq(endpoint);
    pq.bind();

    pq.runRecieveThread();

    std::string clientEndpoint;

    auto msg = pq.pull();
    try {
        auto auth_msg = std::dynamic_pointer_cast<AuthorizationWithName>(msg);
        clientEndpoint = auth_msg->endpoint;
    } catch (...) {
        std::cout << "Hui!\n"; exit(-1);
    }
    
    PushQueue ps(clientEndpoint);

    ps.connect();
    ps.runDispatchThread();

    auto resp = Message::construct<AuthorizationAccepted>();
    ps.push(resp);

    while(true) {
        auto msg = pq.pull();
        std::cout << serialize(msg) << std::endl;
    }

    pq.stop();

    return 0;
}