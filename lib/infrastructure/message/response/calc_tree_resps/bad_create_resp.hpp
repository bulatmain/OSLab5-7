#ifndef BAD_CREATE_RESP_HPP
#define BAD_CREATE_RESP_HPP

#include "../response.hpp"

namespace lab5_7 {
    class BadCreateResp : public Response {
    public:
        uint64_t newNodeId;
        uint64_t parentId;

    public:
        BadCreateResp(uint64_t newNodeId, uint64_t parentId) : newNodeId(newNodeId), parentId(parentId) {}

        static std::shared_ptr<BadCreateResp> deserialize(std::string& resp_str) {
            if (extractType(resp_str) != "BadCreateResp") {
                throw std::invalid_argument("Error: attempt to deserialize invalid string");
            }
            return deserializeUnpacked(resp_str);
        }
        static std::shared_ptr<BadCreateResp> deserialize(std::string&& resp_str) {
            auto _resp_str = std::move(resp_str);
            return deserialize(resp_str);
        }
        static std::shared_ptr<BadCreateResp> deserialize(std::string const& resp_str) {
            auto _resp_str = resp_str;
            return deserialize(resp_str);
        }

        static std::shared_ptr<BadCreateResp> deserializeUnpacked(std::string& resp_str) {
            auto pos = find_start_of_class_vars(resp_str);
            auto newNodeId = getNextVar<uint64_t>(resp_str, pos);
            auto parentId = getNextVar<uint64_t>(resp_str, pos);
            return std::make_shared<BadCreateResp>(newNodeId, parentId);
        }

    protected:
        virtual void serialize_response(std::string& resp_str) const override final {
            add_invalid_request_header(resp_str);
            add_first_class_variable(resp_str, newNodeId);
            add_next_class_variable(resp_str, parentId);
            complete_serialization(resp_str);
        }

        void add_invalid_request_header(std::string& resp_str) const {
            resp_str += "BadCreateResp{";
        }

    };
};


#endif