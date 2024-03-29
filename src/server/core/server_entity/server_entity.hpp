#ifndef SERVER_ENTITY_HPP
#define SERVER_ENTITY_HPP

#include "validator/validator.hpp"
// #include <calc_tree/calc_tree_module.hpp>
#include "calc_tree_dummy.hpp"

#include <chrono>
#include <thread>

namespace lab5_7 {
    using namespace std::chrono_literals;

    class Server final {
    private:
        using duration_ms = std::chrono::milliseconds;
        using eventsToHandleList = std::shared_ptr<ConcurrentQueue<Message>>;
        using thread_ptr = std::shared_ptr<std::thread>;
        using pusher_ptr = std::shared_ptr<PushQueue>;
        using flag_ptr   = std::shared_ptr<FlagVO>;


        int const listeningPort;
        std::string const selfEndpoint;
        flag_ptr keep_running;

        duration_ms const passingTime;

        eventsToHandleList eventsList;

        CalcTreeInterface calcTreeInterface;
        thread_ptr calcTreeThread;
        thread_ptr validatorThread;
        pusher_ptr pusher;
        

    public:
        Server(int listeningPort = 5000, duration_ms passingTime = 1000ms, duration_ms heartbeatPeriod = 1000ms)
            :   listeningPort(listeningPort), 
                selfEndpoint(SelfIPInfo::getSelfEndpoint(listeningPort)), 
                keep_running(std::make_shared<FlagVO>(false)),
                passingTime(passingTime),
                eventsList(std::make_shared<ConcurrentQueue<Message>>()),
                calcTreeInterface(eventsList, keep_running, heartbeatPeriod) {
            
            std::cout << "Self endpoint is: " << selfEndpoint << std::endl;
        }

        void launch() {
            keep_running->setValue(true);
            launchCalcTreeThread();
            launchValidatorThread();
            run();
        }

        void stop() {
            keep_running->setValue(false);
            validatorThread->join();
            calcTreeThread->join();
        }

        ~Server() {
            stop();
        }

    private:
        void launchCalcTreeThread() {
            auto calcTree = CalcTree::makePointer(calcTreeInterface);
            calcTreeThread = std::make_shared<std::thread>(calcTreeCallLaunch, calcTree);
            //makeThread<CalcTree>(calcTreeCallLaunch, calcTree);
        }

        static void calcTreeCallLaunch(CalcTree::ct_ptr calcTree) {
            calcTree->launch();
        }

        void launchValidatorThread() {
            auto validator = Validator::makePointer(selfEndpoint, eventsList, keep_running);
            validatorThread = std::make_shared<std::thread>(validatorCallLaunch, validator);
            //makeThread<Validator>(validatorCallLaunch, validator);
        }

        static void validatorCallLaunch(Validator::vdtr_ptr validator) {
            validator->launch();
        }

        template <typename Object>
        thread_ptr makeThread(std::function<void(std::shared_ptr<Object>)> launcher, std::shared_ptr<Object> object) {
            return std::make_shared<std::thread>(launcher, object);
        }

        void run() {
            passForTime(passingTime);
            while (keep_running->getValue()) {
                if (thereIsWhatToHandle()) {
                    auto msg = getEvent();
                    std::cout << "<< Msg recieved: " << serialize(msg) << std::endl;
                    handle(msg);
                } else {
                    passForTime(passingTime);
                }
            }
        }

        bool thereIsWhatToHandle() {
            return !eventsList->empty();
        }

        Message::msg_ptr getEvent() {
            return eventsList->wait_and_pop();
        }

        void handle(Message::msg_ptr msg) {
            if (isTypeMsg<Request>(msg)) {
                handleRequest(std::dynamic_pointer_cast<Request>(msg));
            } else if (isTypeMsg<Response>(msg)) {
                handleResponse(std::dynamic_pointer_cast<Response>(msg));
            }
        }

        void handleRequest(Request::req_ptr req) {
            if (isTypeMsg<AuthorizationWithName>(req)) {
                authorizeByRequest(std::dynamic_pointer_cast<AuthorizationWithName>(req));
            } else if (isTypeMsg<Command>(req)) {
                redirectToCalcTree(std::dynamic_pointer_cast<Command>(req));
            }
        }

        void authorizeByRequest(AuthorizationWithName::auwn_ptr req) {
            std::cout << "[] Handled: Authorization\n";
            auto const& endpoint = req->endpoint;
            pusher = std::make_shared<PushQueue>(endpoint);
            pusher->connect();
            pusher->runDispatchThread();
            auto succesfullAuthorization = constructMessageType<AuthorizationAccepted>();
            redirectToClient(succesfullAuthorization);
        }

        void redirectToCalcTree(Command::cmd_ptr cmd) {
            std::cout << "[] Handled: Redirecting to CalcTree\n";
            calcTreeInterface.push(cmd);
        }

        void handleResponse(Response::resp_ptr resp) {
            redirectToClient(resp);
        }

        void redirectToClient(Response::resp_ptr resp) {
            std::cout << "[] Handled: Redirecting to Client\n";
            pusher->push(resp);
        }

        void passForTime(duration_ms duration) {
            std::this_thread::sleep_for(duration);
        }

    };
};

#endif