#include "programe_executer.h"

#include <algorithm>

#include "utils/utils.h"

ProgramExecuter::ProgramExecuter(int id, std::string path, std::vector<std::pair<CommandID, int> > program) :
    id_(id),
    config_path_(path),
    program_(program)
{
    json file;
    readJSONFromFile(file,config_path_);

    input_len_ = file["num_inputs_regs"];
    int seed = file["generator"]["seed"];
    if (seed == -1) {
        seed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    std::mt19937 gen(seed);
    gen_ = gen;
    gen_.seed(seed);

    json file_exec = file["executer"];
    inputs_regs_dis_ = std::uniform_int_distribution<>(file_exec["lower_input_bound"],file_exec["upper_input_bound"]);
    max_tests_ = file_exec["max_tests"];
}


void ProgramExecuter::TaskQueryThreadProc() {
    json report;

    Controller controller(config_path_);

    for (int iter=0; iter<max_tests_ && !interrupt_requested_; iter++) {
        json report_sub;

        std::vector<int> input;
        for (int i =0; i< input_len_; i++ )
            input.push_back(inputs_regs_dis_(gen_));

        report_sub["in"] = json(input);

        if (!controller.Init(input,program_)) {
            report_sub["valid"] = false;
            report_sub["error"] = false;
            report_sub["out"] = json::array();
        }
        report_sub["valid"] = true;

        controller.Evaluate();

        if (controller.getInnerState() != ControllerState::Done) {
            report_sub["error"] = true;
            report_sub["out"] = json::array();
        }
        report_sub["error"] = false;
        std::vector<int> out;
        if(!controller.getOutputRegs(out)) {
            report_sub["error"] = true;
            report_sub["out"] = json::array();
        }
        report_sub["out"] = json(out);

        report.push_back( report_sub );
    }

    json unique_res;

    for (json &elem : report) {
        if (!elem["error"] && elem["valid"] && !std::equal(elem["in"].begin(),elem["in"].end(), elem["out"].begin())) {
            if (std::all_of(unique_res.begin(),unique_res.end(), [&](json a) {
                    return !std::equal(a["out"].begin(),a["out"].end(), elem["out"].begin());
            })) {
                json new_elem = {
                    {"in", elem["in"]},
                    {"out", elem["out"]}
                };
                unique_res.push_back(new_elem);
            }
        }
    }

    report_["ID"] = id_;
    report_["enties"] = unique_res;
    report_["num_unique"] = unique_res.size();
    alive_ = false;
}
