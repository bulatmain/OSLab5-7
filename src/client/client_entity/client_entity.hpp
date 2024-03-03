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
            std::cout << ">> Please, enter command: ";
        }

        static string_ptr readLine() {
            auto input_ptr = std::make_shared<std::string>();
            std::getline(std::cin, *input_ptr);
            return input_ptr;
        }
    };

    class Client {
    protected:
        ClientParser parser;

        std::string const selfEndpoint = SelfIPInfo::getSelfEndpoint();
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

        void run() {
            while (true) {
                auto input = Reader::readCommandFromUser();
                try {
                    auto cmd = parser.parse(*input);
                    handle(cmd);
                } catch (...) {
                    printInvalidInputError();
                }
            }
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

        void handle(Command::cmd_ptr cmd) {
            if (isAssignedToThis(cmd)) {
                handleThere(cmd);
            } else {
                pushRequestToQueue(cmd);
                waitAndPullRespFromQueue();
            }
        }

        void printPullQueueMsgIfNotEmpty() {
            if (isThereActualResponse()) {
                waitAndPullRespFromQueue();
            }
        }

        void waitAndPullRespFromQueue() {
            try {
                auto msg = puller.pull();
                std::string msg_str = formAnswerToUser(msg);
                std::cout << msg_str << std::endl;
            } catch (...) {}
        }

        std::string formAnswerToUser(Message::msg_ptr resp) {
            if (isTypeMsg<InvalidRequest>(resp)) {
                return formAnswerToUserForInvalidRequest(std::dynamic_pointer_cast<InvalidRequest>(resp));
            } else if (isTypeMsg<BadExecResp>(resp)) {
                return formAnswerToUserForBadExecResp(std::dynamic_pointer_cast<BadExecResp>(resp));
            } else if (isTypeMsg<GoodExecResp>(resp)) {
                return formAnswerToUserForGoodExecResp(std::dynamic_pointer_cast<GoodExecResp>(resp));
            } else if (isTypeMsg<BadCreateResp>(resp)) {
                return formAnswerToUserForBadCreateResp(std::dynamic_pointer_cast<BadCreateResp>(resp));
            } else if (isTypeMsg<GoodCreateResp>(resp)) {
                return formAnswerToUserForGoodCreateResp(std::dynamic_pointer_cast<GoodCreateResp>(resp));
            } else if (isTypeMsg<NodeExistResponse>(resp)) {
                return formAnswerToUserForNodeExistResponse(std::dynamic_pointer_cast<NodeExistResponse>(resp));
            } else if (isTypeMsg<NodeIsDeadResponse>(resp)) {
                return formAnswerToUserForNodeIsDeadResponse(std::dynamic_pointer_cast<NodeIsDeadResponse>(resp));
            } 
            throw std::runtime_error("Error: can not cast Response to answer!");
        }

        std::string formAnswerToUserForInvalidRequest(std::shared_ptr<InvalidRequest>) {
            return "Error: invalid request";
        }

        std::string formAnswerToUserForBadExecResp(std::shared_ptr<BadExecResp> resp) {
            auto id = resp->id;
            return "Error: " + std::to_string(id) + " node is manager, unavailable or not created";
        }

        std::string formAnswerToUserForGoodExecResp(std::shared_ptr<GoodExecResp> resp) {
            auto id = resp->id;
            auto sum = resp->sum;
            return "Ok:" + std::to_string(id) + ": " + std::to_string(sum);
        }

        std::string formAnswerToUserForBadCreateResp(std::shared_ptr<BadCreateResp> resp) {
            auto id = resp->newNodeId;
            return "Error: unable to create" + std::to_string(id) + " node";
        }

        std::string formAnswerToUserForGoodCreateResp(std::shared_ptr<GoodCreateResp> resp) {
            auto pid = resp->pid;
            return "Ok: " + std::to_string(pid);
        }

        std::string formAnswerToUserForNodeExistResponse(std::shared_ptr<NodeExistResponse> resp) {
            auto id = resp->id;
            return "Error: node" + std::to_string(id) + " already exist";
        }

        std::string formAnswerToUserForNodeIsDeadResponse(std::shared_ptr<NodeIsDeadResponse> resp) {
            auto id = resp->id;
            return "Heartbeat: node " + std::to_string(id) + " is unavailable now";
        }

        void printInvalidInputError() {
            std::cout << "!! Error: invalid input!\n";
        }

        void pushRequestToQueue(Request::req_ptr req) {
            pusher.push(req);
        }

        void waitTillPullerIsNotEmpty(std::size_t& seconds_left) {
            while (!isThereActualResponse()) {
                std::this_thread::sleep_for(1s);
                std::cout << std::to_string(++seconds_left) + " seconds..." << std::endl;
            }
        }

        bool badPulledResponce() {
            auto msg = puller.pull();
            return !msgIsAuthorizationAcceptedType(msg);
        }

        bool isAssignedToThis(Command::cmd_ptr cmd) {
            return listHasElement(clientSideCommandTypeList, cmd->identifyCommand());
        }

        void handleThere(Command::cmd_ptr cmd) {
            auto type = cmd->identifyCommand();
            if (type == CommandType::PassEnum) {
                pass(cmd);
            } else {
                throw std::runtime_error("Error: attempt to handle someone else's command in client handler");
            }
        }

        bool isThereActualResponse() {
            return !puller.empty();
        }

        bool msgIsAuthorizationAcceptedType(Message::msg_ptr msg) {
            auto p = std::dynamic_pointer_cast<AuthorizationAccepted>(msg);
            return static_cast<bool>(p);
        }

        template <typename T>
        bool listHasElement(std::list<T> const& l, T const& v) {
            return std::find(l.begin(), l.end(), v) != l.end();
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

    };

};

#endif