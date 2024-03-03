#ifndef HEARTBEAT_HPP
#define HEARTBEAT_HPP

#include "../response.hpp"
#include <chrono>

namespace lab5_7 {
    class Heartbeat : public Response {
    public:
        using time_point = std::chrono::system_clock::time_point;

        uint64_t nodeId;
        time_point const lastHeartbeatTime;

    public:
        Heartbeat(uint64_t nodeId, time_point lastHeartbeatTime) noexcept 
            : nodeId(nodeId), lastHeartbeatTime(lastHeartbeatTime) {}

        static std::shared_ptr<Heartbeat> deserialize(std::string& msg_str) {
            if (extractType(msg_str) != "Heartbeat") {
                throw std::invalid_argument("Error: trying to deserialize invalid heartbeat string");
            }
            return deserializeUnpacked(msg_str);
        }

        static std::shared_ptr<Heartbeat> deserialize(std::string&& msg_str) {
            auto _ireq_str = std::move(msg_str);
            return deserialize(msg_str);
        }
        static std::shared_ptr<Heartbeat> deserialize(std::string const& msg_str) {
            auto _msg_str = msg_str;
            return deserialize(msg_str);
        }

        static std::shared_ptr<Heartbeat> deserializeUnpacked(std::string const& msg_str) {
            std::size_t pos = find_start_of_class_vars(msg_str);
            auto nodeId = getNextVar<uint64_t>(msg_str, pos);
            auto recordInUint64_t = getNextVar<uint64_t>(msg_str, pos);
            auto lastHeartbeatTime = convertTimePointFromUint64_t(recordInUint64_t);
            return std::make_shared<Heartbeat>(nodeId, lastHeartbeatTime);
        }

    protected:
        virtual void serialize_response(std::string& msg_str) const override final {
            add_auth_with_name_header(msg_str);
            add_first_class_variable(msg_str, nodeId);
            auto recordInUint64_t = convertUint64_tToTimePoint(lastHeartbeatTime);
            add_next_class_variable(msg_str, recordInUint64_t);
            complete_serialization(msg_str);
        }

        void add_auth_with_name_header(std::string& msg_str) const {
            msg_str += "Heartbeat{";
        }

        static time_point convertTimePointFromUint64_t(uint64_t recordInUint64_t) {
            time_point t;
            auto dur = std::chrono::steady_clock::duration(recordInUint64_t);
            return t += dur;
        }

        static uint64_t convertUint64_tToTimePoint(time_point lastHeartbeatTime) {
            return static_cast<uint64_t>(lastHeartbeatTime.time_since_epoch().count());
        }

    };
};

#endif