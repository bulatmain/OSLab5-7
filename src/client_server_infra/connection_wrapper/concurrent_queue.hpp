#ifndef CONCURRENT_QUEUE_HPP
#define CONCURRENT_QUEUE_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

template<typename T>
class ConcurrentQueue {
    public:
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

        void push(T const& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _queue.push(data);
            lock.unlock();
            _condition.notify_one();
        }

        void push(T&& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _queue.push(std::move(data));
            lock.unlock();
            _condition.notify_one();
        }

        std::size_t size() const {
            return _queue.size();
        }

        std::size_t size() {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.size();
        }

        bool empty() {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.empty();
        }

        std::queue<T> copy_queue() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue;
        }

        std::queue<T> unload() {
            std::lock_guard<std::mutex> lock(_mutex);
            std::queue<T> blank;
            std::swap(blank, _queue);
            return blank;
        }       

        bool try_pop(T& value) {
            std::lock_guard<std::mutex> lock(_mutex);

            if (_queue.empty()) {
                return false;
            }

            value = std::move(_queue.front());
            _queue.pop();
            return true;
        }

        void wait_and_pop(T& value) {
            std::unique_lock<std::mutex> lock(_mutex);

            while (_queue.empty()) {
                _condition.wait(lock);
            }

            value = std::move(_queue.front());
            _queue.pop();
        }

        bool wait_and_pop_while(T& value,
            std::chrono::milliseconds timeout_duration=std::chrono::seconds(1),
            const std::chrono::milliseconds& check_interval=std::chrono::milliseconds(10)) {
            std::unique_lock<std::mutex> lock(_mutex);

            while (_queue.empty()) {
                if (_condition.wait_for(lock, check_interval) == std::cv_status::timeout) {
                    timeout_duration -= check_interval;
                    if (timeout_duration <= std::chrono::milliseconds::zero() ) {
                        return false;
                    }
                }
            }

            value = std::move(_queue.front());
            _queue.pop();
            return true;
        }

    private:
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _condition;
};

#endif