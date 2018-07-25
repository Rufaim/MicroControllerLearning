#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <utility>

#include "command_list.h"

enum ControllerState { Created, Ready, isRunning, Done, Error };

class Controller {

public:
    Controller(std::string config_path);

    bool Init(std::vector<int> input, std::vector<std::pair<CommandID,int>> program);

    void Evaluate();
    bool evaluateCommand(CommandID command, int arg);

    bool getOutputRegs(std::vector<int> &out);
    ControllerState getInnerState() { return state_; }

private:
    int num_inputs_regs_;
    int num_active_regs_;
    int num_output_regs_;

    int error_idx_;
    ControllerState state_;

    std::vector<std::pair<CommandID,int>> program_;
    std::vector<int> input_regs_;

    //inner_state
    std::vector<int> output_regs_;
    std::vector<int> work_regs_;
    int accum_;
    int backup_;
    int current_work_reg_idx_;
    int comm_pointer_;

    bool checkArgtypeVal(ArgumentType type, int arg);
};

#endif // CONTROLLER_H
