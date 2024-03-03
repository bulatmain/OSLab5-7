#ifndef BAD_EXEC_RESP_HPP
#define BAD_EXEC_RESP_HPP

#include "../response.hpp"

namespace lab5_7 {
    class BadExecResp : public Response {
    public:
        uint64_t id;

    public:
        BadExecResp(uint64_t id) : id(id) {}

        static std::shared_ptr<BadExecResp> deserialize(std::string& resp_str) {
            if (extractType(resp_str) != "BadExecResp") {
                throw std::invalid_argument("Error: attempt to deserialize invalid string");
            }
            return deserializeUnpacked(resp_str);
        }
        static std::shared_ptr<BadExecResp> deserialize(std::string&& resp_str) {
            auto _resp_str = std::move(resp_str);
            return deserialize(resp_str);
        }
        static std::shared_ptr<BadExecResp> deserialize(std::string const& resp_str) {
            auto _resp_str = resp_str;
            return deserialize(resp_str);
        }

        static std::shared_ptr<BadExecResp> deserializeUnpacked(std::string& resp_str) {
            auto pos = find_start_of_class_vars(resp_str);
            auto id = getNextVar<uint64_t>(resp_str, pos);
            return std::make_shared<BadExecResp>(id);
        }

    protected:
        virtual void serialize_response(std::string& resp_str) const override final {
            add_invalid_request_header(resp_str);
            add_first_class_variable(resp_str, id);
            complete_serialization(resp_str);
        }

        void add_invalid_request_header(std::string& resp_str) const {
            resp_str += "BadExecResp{";
        }

    };
};


#endif