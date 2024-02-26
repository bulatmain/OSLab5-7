#ifndef CONCURRENT_QUEUE_HPP
#define CONCURRENT_QUEUE_HPP

#include <functional>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>

#include "concurrent_queue_exceptions.hpp"

namespace lab5_7 {
    template<typename T>
    class ConcurrentQueue {
        public:
            using t_ptr = std::shared_ptr<T>;
            using t_const_ptr = std::shared_ptr<T const>;
    
            ConcurrentQueue() = default;                                    // default constructor
            ConcurrentQueue(std::initializer_list<T> const& ilist)          // init_list constructor
                : _queue(ilist) {}
            ConcurrentQueue(const ConcurrentQueue&) = delete;               // copy constructor
            ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;    // copy assignment
            ConcurrentQueue(ConcurrentQueue&&) = delete;                    // move constructor
            ConcurrentQueue& operator=(ConcurrentQueue &&) = delete;        // move assignment
    
            void clear() {
                unload();
            }
    
            void push(t_const_ptr data) {
                std::unique_lock<std::mutex> lock(_mutex);
                _queue.emplace_back(data);
                lock.unlock();
                _condition.notify_one();
            }
    
            std::size_t size() const {
                return _queue.size();
            }
    
            std::size_t size() {
                std::lock_guard<std::mutex> lock(_mutex);
                return const_cast<ConcurrentQueue const*>(this)->size();
            }
    
            bool empty() {
                std::lock_guard<std::mutex> lock(_mutex);
                return _queue.empty();
            }
    
            std::list<t_const_ptr> copy_queue() const {
                return _queue;
            }
    
            std::list<t_const_ptr> copy_queue() {
                std::lock_guard<std::mutex> lock(_mutex);
                return const_cast<ConcurrentQueue const*>(this)->copy_queue();
            }
    
            template <typename S>
            std::list<S> map(std::function<S(t_const_ptr)> conversion) const {
                std::list<S> converted;
                for (auto const& each : _queue) {
                    converted.emplace_back(conversion(each));
                }
                return converted;
            }
    
            template <typename S>
            std::list<S> map(std::function<S(t_const_ptr)> conversion) {
                std::lock_guard<std::mutex> lock(_mutex);
                return std::move(const_cast<ConcurrentQueue const*>(this)->map(conversion));
            }
    
            std::list<t_const_ptr> unload() {
                std::lock_guard<std::mutex> lock(_mutex);
                std::list<t_const_ptr> blank;
                std::swap(blank, _queue);
                return blank;
            }       
    
            t_ptr try_pop() {
                std::lock_guard<std::mutex> lock(_mutex);
    
                if (_queue.empty()) {
                    throw BadAttemtPopFromQueue();
                }
    
                auto value = std::move(_queue.front());
                _queue.pop_front();
                return std::const_pointer_cast<T>(value);
            }
    
            t_ptr wait_and_pop() {
                std::unique_lock<std::mutex> lock(_mutex);
    
                while (_queue.empty()) {
                    _condition.wait(lock);
                }
    
                auto value = std::move(_queue.front());
                _queue.pop_front();
                return std::const_pointer_cast<T>(value);
            }
    
            t_ptr wait_and_pop_while(
                std::chrono::milliseconds timeout_duration=std::chrono::seconds(1),
                const std::chrono::milliseconds& check_interval=std::chrono::milliseconds(10)) {
                std::unique_lock<std::mutex> lock(_mutex);
    
                while (_queue.empty()) {
                    if (_condition.wait_for(lock, check_interval) == std::cv_status::timeout) {
                        timeout_duration -= check_interval;
                        if (timeout_duration <= std::chrono::milliseconds::zero() ) {
                            throw BadAttemtPopFromQueueInGivenTime();
                        }
                    }
                }
    
                auto value = std::move(_queue.front());
                _queue.pop_front();
                return std::const_pointer_cast<T>(value);
            }
    
        private:
            std::list<t_const_ptr> _queue;
            std::mutex _mutex;
            std::condition_variable _condition;
    };
    
};


#endif