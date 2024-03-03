#include <message/message_module.hpp>
#include <iostream>

using namespace lab5_7;

int main() {
    auto time = std::chrono::system_clock::now();
    auto msg = Message::construct<Heartbeat>(1, time);

    auto ser_cmd = serialize(msg);

    std::cout << ser_cmd << "\n";

    auto _msg = Message::deserialize(ser_cmd);

    std::cout << serialize(_msg) << "\n";

    return 0;
}