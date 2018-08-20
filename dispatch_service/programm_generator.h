#ifndef PROGRAMM_GENERATOR_H
#define PROGRAMM_GENERATOR_H

#include <random>
#include <vector>
#include <string>

#include "controller/controller.h"


class ProgrammGenerator
{
public:
    ProgrammGenerator(std::string config_path);

    std::vector<std::pair<CommandID,int>> generate();

private:
    int get_random_command_arg(Command command, std::uniform_int_distribution<> pointer_dis);
    CommandID generate_command_id();
    void post_process(std::vector<std::pair<CommandID,int>>& program);

    int num_inputs_regs_;
    int num_active_regs_;
    int num_output_regs_;

    std::mt19937 gen_;
    std::uniform_real_distribution<> prob_;

    std::uniform_int_distribution<> inputs_regs_dis_;
    std::uniform_int_distribution<> active_regs_dis_;
    std::uniform_int_distribution<> output_regs_dis_;

    std::uniform_int_distribution<> const_dis_;
    std::uniform_int_distribution<> command_dis_;

    std::uniform_int_distribution<> program_len_dis_;

    const std::vector<CommandID> service_commands_ = { CommandID::POP,
                     CommandID::LOAD, CommandID::UNLD, CommandID::PUSH, CommandID::REG };
    const std::vector<ArgumentType> post_neglet_arg_type_ = { ArgumentType::NONE, ArgumentType::ACTIVE_REG};
    const ArgumentType post_neglet_arg_type_2_ = ArgumentType::OUTPUT_REG;
};

#endif // PROGRAMM_GENERATOR_H
