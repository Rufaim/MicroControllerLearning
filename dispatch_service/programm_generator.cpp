#include "programm_generator.h"

#include <fstream>
#include <chrono>

#include "json/json.hpp"
#include "controller/command_list.h"
#include "utils/utils.h"

using namespace nlohmann;

ProgrammGenerator::ProgrammGenerator(std::string config_path) {
    json file;
    readJSONFromFile(file,config_path);

    num_inputs_regs_ = file["num_inputs_regs"];
    num_active_regs_ = file["num_actives_regs"];
    num_output_regs_ = file["num_outputs_regs"];

    json file_gen = file["generator"];

    int seed = file_gen["seed"];
    int lower_const_bound = file_gen["lower_const_bound"];
    int upper_const_bound = file_gen["upper_const_bound"];

    int max_program_len = file_gen["max_program_len"];
    int min_program_len = file_gen["min_program_len"];

    if (seed == -1) {
        seed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    std::mt19937 gen(seed);
    gen_ = gen;
    gen_.seed(seed);

    prob_ = std::uniform_real_distribution<>();

    inputs_regs_dis_ = std::uniform_int_distribution<>(0, num_inputs_regs_-1);
    active_regs_dis_ = std::uniform_int_distribution<>(0, num_active_regs_-1);
    output_regs_dis_ = std::uniform_int_distribution<>(0, num_output_regs_-1);

    const_dis_ = std::uniform_int_distribution<>(lower_const_bound, upper_const_bound);
    command_dis_ = std::uniform_int_distribution<>(0, CommandID::NOP);
    program_len_dis_ = std::uniform_int_distribution<>(min_program_len, max_program_len);
}

std::vector<std::pair<CommandID, int>> ProgrammGenerator::generate() {
    int prog_len = program_len_dis_(gen_);

    std::vector<std::pair<CommandID, int>> result;
    std::uniform_int_distribution<> pointer_dis(0,prog_len/2);

    for (int i=0; i<=prog_len; i++) {
        Command command;
        command = getCommandByID( generate_command_id() );
        int arg = get_random_command_arg(command,pointer_dis);
        result.push_back({command.id,arg});

    }
    post_process(result);
    return result;
}

int ProgrammGenerator::get_random_command_arg(Command command,
                                            std::uniform_int_distribution<> pointer_dis) {

    int arg = 0;
    switch (command.argtype) {
    case ArgumentType::INPUT_REG :
        arg = inputs_regs_dis_(gen_);
        break;
    case ArgumentType::ACTIVE_REG :
        arg = active_regs_dis_(gen_);
        break;
    case ArgumentType::OUTPUT_REG :
        arg = output_regs_dis_(gen_);
        break;
    case ArgumentType::CONSTANT :
        arg = const_dis_(gen_);
        break;
    case ArgumentType::POINTER :
        arg = pointer_dis(gen_);
        break;
    }
    return arg;
}

CommandID ProgrammGenerator::generate_command_id() {
    float prob = prob_(gen_);
    if  (prob< 0.2)
        return (CommandID) service_commands_.at(rand() % service_commands_.size());

    return (CommandID) command_dis_(gen_);
}

void ProgrammGenerator::post_process(std::vector<std::pair<CommandID, int> > &program) {
    auto ptr = std::find_if_not(program.begin(),program.end(), [&] (std::pair<CommandID, int>& comm_pair) {
            Command command = getCommandByID( comm_pair.first );
            for (ArgumentType elem : post_neglet_arg_type_) {
                if ( elem == command.argtype)
                    return true;
            }
            return false;
    });
    if (ptr != program.begin())
        program.erase(program.begin(), ptr);


    std::vector<bool> used_output_regs(num_output_regs_,false);
    for (std::pair<CommandID, int>& comm_pair : program) {
        Command command = getCommandByID( comm_pair.first );
        if (command.argtype == ArgumentType::OUTPUT_REG)
            used_output_regs.at(comm_pair.second) = true;
    }

    if (std::all_of(used_output_regs.begin(),used_output_regs.end(), [&](bool a) { return a; })) {
        auto ptr = std::find_if(program.begin(),program.end(), [&] (std::pair<CommandID, int>& comm_pair) {
                Command command = getCommandByID( comm_pair.first );
                return command.argtype == post_neglet_arg_type_2_;
        });
        if (ptr != program.end())
            program.erase(ptr, program.end());
    } else {
        for (int i =0; i< num_output_regs_; i++ ) {
            if (!used_output_regs.at(i)) {
                program.push_back({CommandID::REG,active_regs_dis_(gen_)});
                program.push_back({CommandID::UNLD, i });
            }
        }
    }
}
