#ifndef KILL_SIGNAL_HPP
#define KILL_SIGNAL_HPP

#include "../message.hpp"

namespace lab5_7 {
    class KillSignal : public Message {
    public:    
        static std::shared_ptr<KillSignal> deserialize(std::string& msg_str) {
            if (extractType(msg_str) != "KillSignal") {
                throw std::invalid_argument("Error: trying to deserialize invalid kill_signal string");
            }
            return deserializeUnpacked(msg_str);
        }

        static std::shared_ptr<KillSignal> deserialize(std::string&& msg_str) {
            auto _ireq_str = std::move(msg_str);
            return deserialize(msg_str);
        }
        static std::shared_ptr<KillSignal> deserialize(std::string const& msg_str) {
            auto _msg_str = msg_str;
            return deserialize(msg_str);
        }

        static std::shared_ptr<KillSignal> deserializeUnpacked(std::string const&) {
            return std::make_shared<KillSignal>();
        }

    protected:
        virtual void serialize_message(std::string& msg_str) const override final {
            add_auth_with_name_header(msg_str);
            complete_serialization(msg_str);
        }

        void add_auth_with_name_header(std::string& msg_str) const {
            msg_str += "KillSignal{";
        }

    };
};

#endif