#ifndef GOOD_CREATE_RESP_HPP
#define GOOD_CREATE_RESP_HPP

#include "../response.hpp"

namespace lab5_7 {
    class GoodCreateResp : public Response {
    public:
        uint64_t id;
        uint64_t pid;

    public:
        GoodCreateResp(uint64_t id, uint64_t pid) : id(id), pid(pid) {}

        static std::shared_ptr<GoodCreateResp> deserialize(std::string& resp_str) {
            if (extractType(resp_str) != "GoodCreateResp") {
                throw std::invalid_argument("Error: attempt to deserialize invalid string");
            }
            return deserializeUnpacked(resp_str);
        }
        static std::shared_ptr<GoodCreateResp> deserialize(std::string&& resp_str) {
            auto _resp_str = std::move(resp_str);
            return deserialize(resp_str);
        }
        static std::shared_ptr<GoodCreateResp> deserialize(std::string const& resp_str) {
            auto _resp_str = resp_str;
            return deserialize(resp_str);
        }

        static std::shared_ptr<GoodCreateResp> deserializeUnpacked(std::string& resp_str) {
            auto pos = find_start_of_class_vars(resp_str);
            auto id = getNextVar<uint64_t>(resp_str, pos);
            auto pid = getNextVar<uint64_t>(resp_str, pos);
            return std::make_shared<GoodCreateResp>(id, pid);
        }

    protected:
        virtual void serialize_response(std::string& resp_str) const override final {
            add_invalid_request_header(resp_str);
            add_first_class_variable(resp_str, id);
            add_next_class_variable(resp_str, pid);
            complete_serialization(resp_str);
        }

        void add_invalid_request_header(std::string& resp_str) const {
            resp_str += "GoodCreateResp{";
        }

    };
};


#endif