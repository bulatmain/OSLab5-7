#ifndef CLIENT_ENTITY_HPP
#define CLIENT_ENTITY_HPP

#include "infrastructure_module.hpp"
#include "parser/parser.hpp"
#include "get_ip_func/get_ip_func.hpp"

#include <iostream>

namespace lab5_7 {    
    class Reader {
    protected:
        using string_ptr = std::shared_ptr<std::string>;
        string_ptr input_ptr;
    public:
        Reader() : input_ptr(std::make_shared<std::string>()) {
            readLine();
        }
        Reader(string_ptr str_ptr) : input_ptr(str_ptr) {
            readLine();
        }

        void readLine() {
            std::cout << "Please, enter command: ";
            std::getline(std::cin, *input_ptr);
        }

        std::string& getInput() {
            return *input_ptr;
        }

        ~Reader() {
            input_ptr->clear();
        }
    };

    class Client {
    protected:
        ClientParser parser;

        std::string const selfEndpoint = *getSelfEndpoint();
        PushQueue pusher;
        PullQueue puller;


    public:
        // TODO: Constructors
        Client(std::string const& serverEndpoint)
            : pusher(serverEndpoint), puller(selfEndpoint) {
                std::cout << "Self endpoint: " << selfEndpoint << std::endl;

                pusher.connect();
                pusher.runDispatchThread();

                puller.bind();
                puller.runRecieveThread();
            }

        void launch() {
            // sendSelfEndpointToServer();
            while (true) {
                zmq::message_t endpoint_msg(selfEndpoint);
                pusher.push(selfEndpoint);

                zmq::message_t acceptance;
                if (puller.try_pull(acceptance)) {
                    break;
                } else {
                    std::this_thread::sleep_for(1000ms);
                }
            }
            run();
        }

        void stop() {
            pusher.stop();
            puller.stop();
        }

        ~Client() noexcept {
            stop();
        }

    protected:
        void sendSelfEndpointToServer() {
            zmq::message_t endpoint_msg(selfEndpoint);
            pusher.push(selfEndpoint);
        }

        void run() {
            auto input = std::make_shared<std::string>("");
            while (true) {
                Reader read(input);
                auto cmd = getCommandParsedOrNull(*input);
                if (!cmd) {
                    printInvalidInputError();
                    continue;
                }
                handle(cmd);
                if (isThereActualRespone()) {
                    printPullQueueMsgIfNotEmpty();
                }
            }
        }

        Command::cmd_ptr getCommandParsedOrNull(std::string const& input) {
            Command::cmd_ptr cmd;
            try {
                cmd = parser.parse(input);
            } catch (...) {
                cmd = nullptr;
            }
            return std::move(cmd);
        }

        void handle(Command::cmd_ptr cmd) {
            if (isAssignedToThis(cmd)) {
                handleThere(cmd);
            } else {
                pushRequestToQueue(cmd);
            }
        }

        bool isThereActualRespone() {
            return !puller.empty();
        }

        void handleThere(Command::cmd_ptr cmd) {
            auto type = cmd->identify();
            if (type == CommandType::Pass) {
                pass(cmd);
            } else {
                throw std::runtime_error("Error: attempt to handle someone else's command in client handler");
            }
        }

        bool isAssignedToThis(Command::cmd_ptr cmd) {
            return listHasElement(clientSideCommandTypeList, cmd->identify());
        }
        
        void pushRequestToQueue(Command::cmd_ptr cmd) {
            pusher.push(serialize(cmd));
        }

        void pass(Command::cmd_ptr cmd) {
            if (cmd->identify() != Pass) {
                throw std::runtime_error("Error: attempt to call pass func of not Pass type command");
            }
            auto duration = std::dynamic_pointer_cast<CommandPass>(cmd)->time;
            auto timeBegin = std::chrono::steady_clock::now();
            while (keepWaiting(timeBegin, duration)) {
                printPullQueueMsgIfNotEmpty();
                std::this_thread::sleep_for(100ms);
            }
        }

        bool keepWaiting(std::chrono::time_point<std::chrono::steady_clock> timeBegin, CommandPass::duration_ms durationInMs) {
        typedef std::chrono::milliseconds ms;

            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> timeHasPassed{currentTime - timeBegin};
            std::chrono::microseconds timeHasPassedInMs = std::chrono::duration_cast<ms>(timeHasPassed);        
            return timeHasPassedInMs <= durationInMs;
        }

        void printPullQueueMsgIfNotEmpty() {
            if (!puller.empty()) {
                zmq::message_t msg;
                if (puller.try_pull(msg)) {
                    // std::string msg_str = unpack_responce(msg);
                    std::string msg_str = msg.to_string();
                    std::cout << msg_str << std::endl;
                }
            }
        }

        template <typename T>
        bool listHasElement(std::list<T> const& l, T const& v) {
            return std::find(l.begin(), l.end(), v) != l.end();
        }

        void printInvalidInputError() {
            std::cout << "Error: invalid input!\n";
        }

    };

};

#endif