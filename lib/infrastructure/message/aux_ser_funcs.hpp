#ifndef AUXILIARY_SERIALIZATION_FUNCTIONS
#define AUXILIARY_SERIALIZATION_FUNCTIONS

#include <sstream>
#include <string>

namespace lab5_7 {
    std::string extractType(std::string& ser_msg) {
        std::size_t pos = ser_msg.find('{');
        if (pos == std::string::npos) {
            return "";
        }
        auto type = ser_msg.substr(0, pos); 
        ser_msg = ser_msg.substr(pos + 1, ser_msg.size() - pos - 2);
        return type;
    }

    template <typename T>
    void add_first_class_variable(std::string& msg_str, T const& var) {
        msg_str += std::to_string(var);
    }
    void add_first_class_variable(std::string& msg_str, std::string const& var) {
        msg_str += var;
    }
    template <typename T>
    void add_next_class_variable(std::string& msg_str, T const& var) {
        msg_str += ", " + std::to_string(var);
    }
    void add_next_class_variable(std::string& msg_str, std::string const& var) {
        msg_str += ", " + var;
    }
    void complete_serialization(std::string& msg_str) {
        msg_str += "}";
    }

    std::size_t find_start_of_class_vars(std::string const& msg_str) {
        return msg_str.find('{') + 1;
    }

    std::size_t findRightEdgeOfNextVar(std::string const& msg_str, std::size_t& l) {
        return msg_str.find(',', l);
    }

    std::string getNextVarString(std::string const& msg_str, std::size_t& l) {
        std::size_t r = findRightEdgeOfNextVar(msg_str, l);
        if (r == std::string::npos) {
            std::size_t l_ = msg_str.size(); std::swap(l, l_);
            return msg_str.substr(l_, msg_str.size() - l_);
        } else {
            std::size_t l_ = r + 2; std::swap(l, l_);
            return msg_str.substr(l_, r - l_);
        }
    }

    template <typename T>
    T convert_string(std::string const& str) {
        std::istringstream converter(str);
        T var; converter >> var;
        return var;
    }

    template <typename T>
    T getNextVar(std::string const& msg_str, std::size_t& l) {
        std::string var_str = getNextVarString(msg_str, l);
        return convert_string<T>(var_str);
    }

};

#endif