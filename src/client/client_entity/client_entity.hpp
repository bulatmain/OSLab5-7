#ifndef CLIENT_ENTITY_HPP
#define CLIENT_ENTITY_HPP

#include <infrastructure/connection_wrapper/connection_module.hpp>
#include "parser/parser.hpp"
#include <aux_network_funcs/get_ip_func/get_ip_func.hpp>

#include <iostream>

namespace lab5_7 {    
    class Reader
     {
    protected:
        using string_ptr = std::shared_ptr<std::string>;
    public:
        static string_ptr readCommandFromUser() {
            printInvitingMsg();
            return readLine();
        }

    protected:
        Reader() = default;

        static void printInvitingMsg() {
            std::cout << "Please, enter command: ";
        }

        static string_ptr readLine() {
            string_ptr input_ptr;
            std::getline(std::cin, *input_ptr);
            return input_ptr;
        }
    };

    class Client {
    protected:
        ClientParser parser;

        std::string const selfEndpoint = *getSelfEndpoint();
        std::string const clientName;
        PushQueue pusher;
        PullQueue puller;

    public:
        // TODO: Constructors
        Client(std::string const& serverEndpoint, std::string const& clientName)
            : clientName(clientName), pusher(serverEndpoint), puller(selfEndpoint) {
                std::cout << "Self endpoint: " << selfEndpoint << std::endl;

                pusher.connect();
                pusher.runDispatchThread();

                puller.bind();
                puller.runRecieveThread();
            }

        void launch() {
            connectWithServer();
            run();
        }

        void stop() {
            pusher.stop();
            puller.stop();
        }

        ~Client() {
            stop();
        }

    protected:
        void connectWithServer() {
            pushAuthMessage();
            waitTillAuthAccepted();
        }

        void pushAuthMessage() {
            auto auth_msg = constructMessageType<AuthorizationWithName>(selfEndpoint, clientName);
            pushRequestToQueue(auth_msg);
        }

        void waitTillAuthAccepted() {
            std::cout << "Connecting to server...\n";
            std::size_t seconds_left = 0;
            do {
                waitTillPullerIsNotEmpty(seconds_left);
            } while (badPulledResponce());
        }

        void waitTillPullerIsNotEmpty(std::size_t& seconds_left) {
            while (puller.empty()) {
                std::this_thread::sleep_for(1s);
                std::cout << std::to_string(++seconds_left) + " seconds..." << std::endl;
            }
        }

        bool msgIsAuthorizationAcceptedType(Message::msg_ptr msg) {
            auto p = std::dynamic_pointer_cast<AuthorizationAccepted>(msg);
            if (p) {
                return true;
            } else {
                return false;
            }
        }

        bool badPulledResponce() {
            auto msg = puller.pull();
            return !msgIsAuthorizationAcceptedType(msg);
        }

        void run() {
            while (true) {
                auto input = Reader::readCommandFromUser();
                try {
                    auto cmd = parser.parse(*input);
                    handle(cmd);
                    if (isThereActualRespone()) {
                        printPullQueueMsgIfNotEmpty();
                    }
                } catch (...) {
                    printInvalidInputError();
                }
            }
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
            auto type = cmd->identifyCommand();
            if (type == CommandType::PassEnum) {
                pass(cmd);
            } else {
                throw std::runtime_error("Error: attempt to handle someone else's command in client handler");
            }
        }

        bool isAssignedToThis(Command::cmd_ptr cmd) {
            return listHasElement(clientSideCommandTypeList, cmd->identifyCommand());
        }
        
        void pushRequestToQueue(Request::req_ptr req) {
            pusher.push(req);
        }

        void pass(Command::cmd_ptr cmd) {
            if (cmd->identifyCommand() != PassEnum) {
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
                try {
                    auto msg = puller.try_pull();
                    // std::string msg_str = unpack_responce(msg);
                    std::cout << serialize(msg) << std::endl;
                } catch (...) {}
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