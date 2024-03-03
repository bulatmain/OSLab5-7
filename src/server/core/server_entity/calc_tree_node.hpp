#ifndef CALC_TREE_NODE_HPP
#define CALC_TREE_NODE_HPP

#include <infrastructure/connection_wrapper/connection_module.hpp>
#include <aux_network_funcs/get_ip_func/get_ip_func.hpp>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <filesystem>

namespace lab5_7 {
    struct CommandRecord {
    public:
        CommandType type;
        uint64_t recievedResponses = 0;
        bool wasGoodResponse = false;

        CommandRecord(CommandType type)
            : type(type) {}

        void accountBadResp() {
            ++recievedResponses;
        }

        void accountGoodResp() {
            ++recievedResponses;
            wasGoodResponse = true;
        }

    };

    class CalcNodeRoot {
    public:
        typedef std::chrono::system_clock timer;
        std::string const endpoint;
        PushQueue pusher;
        size_t id;
        size_t pid;
        timer::duration const checkingHeartbeatPeriod;
        timer::time_point lastHeartBeat;
    
        CalcNodeRoot(std::string const& serverEndpoint, std::string const& endpoint, size_t id, timer::duration checkingHeartbeatPeriod)
            :   endpoint(endpoint),
                pusher(endpoint), 
                id(id),
                checkingHeartbeatPeriod(checkingHeartbeatPeriod),
                lastHeartBeat(std::chrono::system_clock::now()) {
            createNodeProcess(serverEndpoint, endpoint, id, checkingHeartbeatPeriod);
            std::this_thread::sleep_for(1s);
            pusher.connect();
            pusher.runDispatchThread();
        }
    
        void push(Message::msg_ptr msg) {
            pusher.push(msg);
        }
    
        void createNodeProcess(std::string const& serverEndpoint, std::string const& endpoint, size_t id, timer::duration checkingHeartbeatPeriod) {
            auto pid = fork();
            if (pid < 0) {
                throw std::runtime_error("Error: can not create CalcTreeNode process");
            } else if (pid == 0) {
                std::string path = "/home/bulat/mai/studying/sem3/OSLabs/Lab5-7/build/src/server/core/calc_tree_node";
                auto id_str = std::to_string(id);
                auto checkingHeartbeatPeriod_str = std::to_string(checkingHeartbeatPeriod.count());
                char* const argv[] = {
                    const_cast<char*>(path.c_str()),
                    const_cast<char*>(serverEndpoint.c_str()),
                    const_cast<char*>(endpoint.c_str()),
                    const_cast<char*>(id_str.c_str()),
                    const_cast<char*>(checkingHeartbeatPeriod_str.c_str()),
                    NULL
                };
                execv(path.c_str(), argv);
            } else {
                this->pid = static_cast<uint64_t>(pid);
            }
        }

        ~CalcNodeRoot() {
            auto killSig = constructMessageType<KillSignal>();
            push(killSig);
        }

    };

};



#endif