#include <iostream>

#include "controller/controller.h"
#include "utils/utils.h"
#include "programe_executer.h"

int main() {

    std::vector<std::pair<CommandID, int> > program;

    readProgramFromFile(program,"/media/pavel/Новый том/data/programms/program_17.txt");

    Controller controller("/media/pavel/Новый том/data/programms/config.json");
    if (controller.Init({9}, program))
        std::cout << "Valid program" << std::endl;

    controller.Evaluate();

    std::vector<int> out_regs;

    if (controller.getOutputRegs(out_regs))
        std::cout << "out received " << out_regs.at(0) << std::endl;

    return 0;
}
