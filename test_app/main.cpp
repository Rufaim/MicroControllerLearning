#include <iostream>

#include "controller/controller.h"
#include "utils/utils.h"
#include "programe_executer.h"

int main() {

    std::vector<std::pair<CommandID,int>> program = {
        { CommandID::LOAD, 0 },
        { CommandID::ADD, 0 },
        { CommandID::MULC, -1 },
        { CommandID::POP, 0 },
        { CommandID::UNLD, 0 }
    };

    ProgramExecuter executer(1,"/home/pavel/New_programs/MicroController/configs/generator.json", program);
    executer.TaskQueryThreadProc();


    //std::vector<std::pair<CommandID,int>> program2;

    /*
    writeProgramToFile(program,"./test.txt");
    readProgramFromFile(program2,"test.txt");

    Controller controller("/home/pavel/New_programs/MicroController/configs/generator.json");
    if (controller.Init({10}, program2))
        std::cout << "Valid program" << std::endl;

    controller.Evaluate();

    std::vector<int> out_regs;

    if (controller.getOutputRegs(out_regs))
        std::cout << "out received " << out_regs.at(0) << std::endl;
    */
    return 0;
}
