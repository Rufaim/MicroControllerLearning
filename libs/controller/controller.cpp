#include "controller.h"

#include <fstream>

#include "json/json.hpp"

using namespace nlohmann;

Controller::Controller(std::string config_path) :
    accum_(0),
    backup_(0),
    current_work_reg_idx_(0),
    comm_pointer_(0),
    error_idx_(-1),
    state_(ControllerState::Created)
{
    std::fstream out(config_path);

    json file;
    out >> file;

    num_inputs_regs_ = file["num_inputs_regs"];
    num_active_regs_ = file["num_actives_regs"];
    num_output_regs_ = file["num_outputs_regs"];
}

bool Controller::Init(std::vector<int> input, std::vector<std::pair<CommandID, int> > program) {
    if (input.size() != num_inputs_regs_)
        return false;

    for ( std::pair<CommandID, int> &elem : program) {
        Command comm = getCommandByID(elem.first);
        if (!checkArgtypeVal(comm.argtype,elem.second))
            return false;
    }

    input_regs_.resize(num_inputs_regs_);
    input_regs_.assign(input.begin(),input.end());
    work_regs_.resize(num_active_regs_);
    output_regs_.resize(num_output_regs_);
    program_.insert(program_.begin(),program.begin(),program.end());
    accum_=0,
    backup_=0,
    current_work_reg_idx_=0,
    comm_pointer_=0,
    error_idx_=-1,
    state_ = ControllerState::Ready;
    return true;
}

void Controller::Evaluate() {
    if (state_ != ControllerState::Ready)
        return;

    for ( ; comm_pointer_ < program_.size(); comm_pointer_++) {
        std::pair<CommandID, int> elem  =  program_.at(comm_pointer_);
        if (!evaluateCommand(elem.first,elem.second)) {
            state_ = ControllerState::Error;
            error_idx_ = comm_pointer_;
            break;
        }
    }
    if (state_ != Error)
        state_ = ControllerState::Done;
}

bool Controller::evaluateCommand(CommandID command, int arg) {
    if (state_ == ControllerState::Error)
        return false;
    if (state_ < ControllerState::isRunning)
        state_ = ControllerState::isRunning;
    try {

        switch (command) {
        case CommandID::NOP:
            return true;
        case CommandID::SAV:
            backup_ = accum_;
            return true;
        case CommandID::NEG:
            accum_ = -accum_;
            return true;
        case CommandID::SWP: {
            int temp = backup_;
            backup_ = accum_;
            accum_ = temp;
            return true;
        }

        case CommandID::ADD:
            accum_ += work_regs_.at(arg);
            return true;
        case CommandID::SUB:
            accum_ -= work_regs_.at(arg);
            return true;
        case CommandID::MUL:
            accum_ *= work_regs_.at(arg);
            return true;
        case CommandID::ADDC:
            accum_ += arg;
            return true;
        case CommandID::SUBC:
            accum_ -= arg;
            return true;
        case CommandID::MULC:
            accum_ *= arg;
            return true;

        case CommandID::REG:
            current_work_reg_idx_ = arg;
            return true;
        case CommandID::PUSH:
            accum_ = work_regs_.at(arg);
            return true;
        case CommandID::POP:
            work_regs_.at(arg) = accum_;
            return true;
        case CommandID::PUSHC:
            accum_ = arg;
            return true;
        case CommandID::LOAD:
            work_regs_.at(current_work_reg_idx_) = input_regs_.at(arg);
            return true;
        case CommandID::UNLD:
            output_regs_.at(arg) = work_regs_.at(current_work_reg_idx_);
            return true;

        case CommandID::JRO:
            comm_pointer_ += arg;
            return true;
        case CommandID::JEZ:
            if (accum_ == 0)
                comm_pointer_ += arg;
            return true;
        case CommandID::JNZ:
            if (accum_ != 0)
                comm_pointer_ += arg;
            return true;
        case CommandID::JGZ:
            if (accum_ > 0)
                comm_pointer_ += arg;
            return true;
        case CommandID::JLZ:
            if (accum_ < 0)
                comm_pointer_ += arg;
            return true;

        default:
            return false;
        }
    }
    catch (const std::exception& e) {
        return false;
    }
}

bool Controller::getOutputRegs(std::vector<int> &out) {
    if (state_ == ControllerState::Error)
        return false;
    out.clear();
    out.assign(output_regs_.begin(),output_regs_.end());
    return true;
}

bool Controller::checkArgtypeVal(ArgumentType type, int arg) {

    switch (type) {
    case ArgumentType::INPUT_REG :
        return 0 <= arg && arg < num_inputs_regs_;
    case ArgumentType::ACTIVE_REG :
        return 0 <= arg && arg < num_active_regs_;
    case ArgumentType::OUTPUT_REG :
        return 0 <= arg && arg < num_output_regs_;
    case ArgumentType::POINTER :
        return 0 <= arg;
    default:
        return true;
    }
}

