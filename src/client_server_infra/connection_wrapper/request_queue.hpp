#ifndef REQUEST_QUEUE_HPP
#define REQUEST_QUEUE_HPP

#include "push_queue.hpp"
#include "auxiliary_entities/push_connection.hpp"

namespace lab5_7 {
    using namespace std::chrono_literals;
    class RequestQueue : public PushQueue {
    protected:
        using thread_ptr = std::shared_ptr<std::thread>;
        using socket_ptr = std::shared_ptr<PushConnection>;
        using flag_ptr = std::shared_ptr<FlagVO>;
        using delay_ms = std::chrono::milliseconds;

        thread_ptr dispatch_thread_ptr;
        delay_ms request_delay;
        socket_ptr pusher;
        flag_ptr keep_running = std::make_shared<FlagVO>(false);

    public:
        RequestQueue(std::string const& endpoint, delay_ms request_delay = 100ms)
            : pusher(std::make_shared<PushConnection>(endpoint)), request_delay(request_delay) {}

        RequestQueue(std::string&& endpoint, delay_ms request_delay = 100ms)
            : pusher(std::make_shared<PushConnection>(std::move(endpoint))), request_delay(request_delay) {}

        void set_endpoint(std::string const& endpoint) {
            pusher->changeEndpoint(endpoint);
        }
        void set_endpoint(std::string&& endpoint) {
            pusher->changeEndpoint(std::move(endpoint));
        }

        void connect() {
            pusher->connect();
        }

        void disconnect() {
            pusher->disconnect();
        }
         
        void runDispatchThread() {
            if (keep_running->getValue()) {
                throw AttemptToLaunchAlreadyRunningThread();
            }
            keep_running->setValue(true);
            dispatch_thread_ptr = std::make_shared<std::thread>(
                _runDispatchThread,
                que_ptr, pusher, keep_running, request_delay
            );
        }

        void stopWhenQueueEmpty() {
            while (!que_ptr->empty()) {}
            stop();
        }

        void stop() {
            if (!keep_running->getValue()) {
                throw AttemptToStopNotRunningThread();
            }
            keep_running->setValue(false);
            join_thread();
        }

    private:
        void join_thread() {
            dispatch_thread_ptr->join();
            dispatch_thread_ptr.reset();
        }

        static void _runDispatchThread(queue_ptr queue, socket_ptr pusher, flag_ptr keep_running, delay_ms request_delay) {
            while (keep_running->getValue()) {
                if (queue->empty()) {
                    std::this_thread::sleep_for(request_delay);
                    continue;
                } else {
                    zmq::message_t msg;
                    queue->wait_and_pop(msg);
                    pusher->push(msg);
                }
            }
        }  

    };
};

#endif