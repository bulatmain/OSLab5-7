#ifndef PULL_QUEUE_HPP
#define PULL_QUEUE_HPP

#include "msg_queue.hpp"
#include "auxiliary_entities/pull_connection.hpp"

#include <chrono>

namespace lab5_7 {
    using namespace std::chrono_literals;
    class PullQueue : public MsgQueue {
    protected:
        using thread_ptr = std::shared_ptr<std::thread>;
        using socket_ptr = std::shared_ptr<PullConnection>;
        using flag_ptr = std::shared_ptr<FlagVO>;
        using delay_ms = std::chrono::milliseconds;

        thread_ptr recieve_thread_ptr;
        delay_ms responce_delay;
        socket_ptr puller;
        flag_ptr keep_running = std::make_shared<FlagVO>(false);

    public:
        PullQueue(std::string const& endpoint, delay_ms responce_delay = 100ms)
            : puller(std::make_shared<PullConnection>(endpoint)), responce_delay(responce_delay) {}

        PullQueue(std::string&& endpoint, delay_ms responce_delay = 100ms)
            : puller(std::make_shared<PullConnection>(std::move(endpoint))), responce_delay(responce_delay) {}

        void pull(zmq::message_t& msg) {
            que_ptr->wait_and_pop(msg);
        }

        bool try_pull(zmq::message_t& msg) {
            return que_ptr->try_pop(msg);
        }

        void set_endpoint(std::string const& endpoint) {
            puller->changeEndpoint(endpoint);
        }
        void set_endpoint(std::string&& endpoint) {
            puller->changeEndpoint(std::move(endpoint));
        }

        void bind() {
            puller->bind();
            
        }

        void unbind() {
            puller->unbind();
        }
         
        bool empty() {
            return que_ptr->empty();
        }

        void runRecieveThread() {
            if (keep_running->getValue()) {
                throw AttemptToLaunchAlreadyRunningThread();
            }
            keep_running->setValue(true);
            recieve_thread_ptr = std::make_shared<std::thread>(
                _runRecieveThread,
                que_ptr, puller, keep_running, responce_delay
            );
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
            recieve_thread_ptr->join();
            recieve_thread_ptr.reset();
        }

        static void _runRecieveThread(queue_ptr queue, socket_ptr puller, flag_ptr keep_running, delay_ms responce_delay) {
            while (keep_running->getValue()) {
                zmq::message_t msg;
                puller->pullDontWait(msg);
                auto res = puller->getPullResult();
                if (res->has_value()) {
                    queue->push(std::move(msg));
                } else {
                    std::this_thread::sleep_for(responce_delay);
                }
            }
        }  

    };
};

#endif