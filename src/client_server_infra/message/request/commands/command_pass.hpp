#ifndef COMMAND_PASS_HPP
#define COMMAND_PASS_HPP

#include "command.hpp"
#include <chrono>

template <class CharT, class Traits>
std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& in, std::chrono::milliseconds& duration) {
    double ms_str;
    in >> ms_str; ms_str /= 1000;
    std::chrono::duration<double> dsecs(ms_str);
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(dsecs);
    return in;
}

namespace lab5_7 {
    class CommandPass : public Command {
    public:
        using duration_ms = std::chrono::milliseconds;
        duration_ms time;
    public:
        CommandPass(duration_ms time) noexcept
            : time(time)  {}

        virtual CommandType identify() const {
            return CommandType::Pass;
        }

        static Command::cmd_ptr deserialize(std::string const& ser_cmd) {
            std::size_t pos = find_start_of_class_vars(ser_cmd);
            duration_ms time = getNextVar<duration_ms>(ser_cmd, pos);
            return std::make_shared<CommandPass>(time);
        }

    protected:
        virtual void serialize_command(std::string& ser_cmd) const override final {
            add_command_type(ser_cmd);
            add_first_class_variable(ser_cmd, time.count());
            complete_serialization(ser_cmd);
        }

        virtual void add_command_type(std::string& ser_cmd) const override final {
            ser_cmd += "Pass";
        }

    };
};

#endif