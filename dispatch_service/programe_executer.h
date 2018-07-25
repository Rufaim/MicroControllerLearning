#ifndef PROGRAME_EXECUTER_H
#define PROGRAME_EXECUTER_H

#include <string>
#include <vector>

#include "task_executer.h"
#include "controller/controller.h"
#include "json/json.hpp"

using namespace nlohmann;

class ProgramExecuter : public TaskExecuter
{
public:
    ProgramExecuter(int id, std::string config_path, std::vector<std::pair<CommandID,int>> program);

    void TaskQueryThreadProc();

    json getReport() { return report_; }

private:
    int id_;
    std::string config_path_;
    int input_len_;
    int max_tests_;
    std::vector<std::pair<CommandID,int>> program_;

    std::mt19937 gen_;
    std::uniform_int_distribution<> inputs_regs_dis_;

    json report_;
};

#endif // PROGRAME_EXECUTER_H
