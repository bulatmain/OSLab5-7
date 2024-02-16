#ifndef CLIENT_PARSER_HPP
#define CLIENT_PARSER_HPP

#include <request/commands/commands_module.hpp>

#include <list>
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
            } else {
                throw std::invalid_argument("Wtf?");
            }
            parse_happened = true;
        }

        Command::cmd_ptr tryParseCreate(std::string const& input) {
            throw std::runtime_error("Create");
        }

        Command::cmd_ptr tryParseExec(std::string const& input) {
            return std::make_shared<CommandExec>(1, 1, 2, 3);
        }

        Command::cmd_ptr tryParsePrintTree(std::string const& input) {
            throw std::runtime_error("PrintTree");
        }

    };
};

#endif