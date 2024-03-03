#ifndef CLIENT_MAIN_HPP
#define CLIENT_MAIN_HPP

#include "client_entity/client_entity.hpp"

using namespace lab5_7;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Error: invalid arguments!\n";
        exit(-1);
    }

    std::string serverEndpoint(argv[1]);

    Client client(serverEndpoint, "bulat");

    client.launch();

    return 0;
}

#endif