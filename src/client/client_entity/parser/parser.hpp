#ifndef CLIENT_PARSER_HPP
#define CLIENT_PARSER_HPP

#include <infrastructure/message/message_module.hpp>

#include <sstream>
#include <stdexcept>

namespace lab5_7 {
    class ClientParser {
    protected:
        Command::cmd_ptr res;
        bool parse_happened = false;

    public:
        Command::cmd_ptr parse(std::string const& input) {
            for (auto const& type : commandTypeList) try {
                if (parse_happened) {
                    break;
                } else {
                    tryParse(type, input);
                }
            } catch (...) {}
            if (!parse_happened) {
                throw std::invalid_argument("Error: invalid input");
            } else {
                return std::move(giveResultAndReset());
            }
        }
    
    protected:
        Command::cmd_ptr giveResultAndReset() {
            auto _res = std::move(res);
            reset();
            return _res;
        }

        void reset() {
            res.reset();
            parse_happened = false;
        }

        void tryParse(CommandType type, std::string const& input) {
            if (type == Create) {
                res = std::move(tryParseCreate(input));
            } else if (type == Exec) {
                res = std::move(tryParseExec(input));
            } else if (type == PrintTree) {
                res = std::move(tryParsePrintTree(input));
            } else if (type == Pass) {
                res = std::move(tryParsePass(input));
            } else {
                throw std::invalid_argument("Wtf?");
            }
            parse_happened = true;
        }

        void check_command(std::istringstream& iss, std::string const& command_name) {
            std::string _command_name;
            iss >> _command_name;
            if (_command_name != command_name) {
                throw std::runtime_error("Error: given input is not " + command_name + " type");
            }
        }

        void check_if_stream_is_empty(std::istringstream& iss) {
            char _; iss >> _;
            if (!iss.eof()) {
                throw std::invalid_argument("Error: invalid input");
            }
        }

        template <typename T>
        void read_var_or_throw_exception(std::istringstream& iss, T& var) {
            if (!(iss >> var)) {
                throw std::invalid_argument("Error: invalid input");
            }
        }

        template <class Arg>
        void read_vars_or_throw_exception(std::istringstream& iss, Arg& arg) {
            read_var_or_throw_exception(iss, arg);
        }

        template <class First, class... Args>
        void read_vars_or_throw_exception(std::istringstream& iss, First& arg, Args&... args) {
            read_var_or_throw_exception(iss, arg);
            read_vars_or_throw_exception(iss, args...);
        }

        Command::cmd_ptr tryParseCreate(std::string const& input) {
            std::istringstream iss(input);
            check_command(iss, "create");
            try {
                uint16_t newNodeId, parentId;
                read_vars_or_throw_exception(iss, newNodeId, parentId);
                check_if_stream_is_empty(iss);
                return constructMessageType<CommandCreate>(newNodeId, parentId);
            } catch (...) {
                throw std::runtime_error("Error: given input is not create type");
            }
        }

        template <typename T>
        void read_vector_or_throw_exception(std::istringstream& iss, std::vector<double>& v) {
            for (auto& each : v) {
                read_var_or_throw_exception(iss, each);
            }
        }

        Command::cmd_ptr tryParseExec(std::string const& input) {
            std::istringstream iss(input);
            check_command(iss, "exec");
            try {
                uint16_t id; std::size_t n;
                read_vars_or_throw_exception(iss, id, n);
                std::vector<double> k(n);
                read_vector_or_throw_exception<double>(iss, k);
                check_if_stream_is_empty(iss);
                return constructMessageType<CommandExec>(id, std::move(k));
            } catch (...) {
                throw std::runtime_error("Error: given input is not exec type");
            }
        }


        Command::cmd_ptr tryParsePrintTree(std::string const& input) {
            std::istringstream iss(input);
            check_command(iss, "print");
            try {
                check_if_stream_is_empty(iss);
                return constructMessageType<CommandPrintTree>();
            } catch (...) {
                throw std::runtime_error("Error: given input is not print type");
            }
        }

        Command::cmd_ptr tryParsePass(std::string const& input) {
            std::istringstream iss(input);
            check_command(iss, "pass");
            try {
                CommandPass::duration_ms time;
                read_var_or_throw_exception(iss, time);
                check_if_stream_is_empty(iss);
                return constructMessageType<CommandPass>(time);
            } catch (...) {
                throw std::runtime_error("Error: given input is not create type");
            }
        }

    };
};

#endif