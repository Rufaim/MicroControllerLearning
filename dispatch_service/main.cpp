#include <iostream>
#include <fstream>
#include <thread>
#include <csignal>
#include <boost/filesystem.hpp>

#include "controller/controller.h"
#include "utils/utils.h"
#include "programm_generator.h"
#include "programe_executer.h"

bool stop = false;

void sig_interrupt();
void SignalHandler(int sig);


int main(int argv, char** argc) {

    std::signal(SIGTERM,&SignalHandler);
    std::signal(SIGINT,&SignalHandler);
    std::signal(SIGQUIT,&SignalHandler);
    int start_id = 0;

    std::string config;
    if (argv > 2)
        start_id = std::atoi(argc[2]);
    if (argv > 1)
        config = argc[1];
    else {
        std::cout << "Usage: ./dispatch_service <config_name> [ <start_id> ]" << std::endl;
        exit(0);
    }
    json conf_file;
    readJSONFromFile(conf_file,config);
    json conf_file_dispatch = conf_file["dispatcher"];

    int num_programms = conf_file_dispatch["num_program"];
    int num_exec = conf_file_dispatch["num_exec"];
    int min_unique = conf_file_dispatch["min_unique"];
    std::string program_safe_dir_str = conf_file_dispatch["dir_to_safe_programs"];
    std::string program_report_file_str = conf_file_dispatch["json_to_save_description"];

    boost::filesystem::path program_safe_dir(program_safe_dir_str);
    boost::filesystem::path program_report_file(program_report_file_str);

    if (!boost::filesystem::exists(program_safe_dir))
        boost::filesystem::create_directories((program_safe_dir));

    boost::filesystem::path path = program_safe_dir;
    path /= "config.json";
    writeJSONToFile(conf_file, path.string());

    ProgrammGenerator generator(config);
    json report;

    std::vector<std::shared_ptr<ProgramExecuter>> executers;
    std::map<int,std::string> active_progs;

    for (int program_id=start_id, processed=0; processed < num_programms && !stop; ) {
        auto res = std::find_if_not(executers.begin(),executers.end(), [&](std::shared_ptr<ProgramExecuter>& exec) {
                return exec->isActive();
        } );

        if (res != executers.end()) {
            (*res)->Stop();
            json rep = (*res)->getReport();
            int num_unique = rep["num_unique"];
            int rep_id = rep["ID"];
            if (num_unique >= min_unique) {
                std::string name = "report_prog" + std::to_string(rep_id) +".json";
                boost::filesystem::path path = program_safe_dir;
                path /= name;
                writeJSONToFile(rep,path.string());

                rep = { {"ID", rep_id},
                        {"num_unique", num_unique},
                        {"program_path", active_progs[rep_id]},
                        {"report_path", path.string()}};

                report.push_back(rep);
            } else {
                boost::filesystem::remove(boost::filesystem::path(active_progs[rep_id]));
            }

            active_progs.erase(rep_id);
            executers.erase(res);
            processed++;
        }

        if (executers.size() < num_exec && program_id<num_programms && !stop) {
            std::cout << "Program #" << program_id << " inited" <<std::endl;
            std::vector<std::pair<CommandID,int>> program = generator.generate();
            std::shared_ptr<ProgramExecuter> exec( new ProgramExecuter (program_id, config,program) );
            executers.push_back(exec);
            exec->Start();

            std::string name = "program_" + std::to_string(program_id)+".txt";
            boost::filesystem::path path = program_safe_dir;
            path /= name;
            writeProgramToFile(program,path.string());
            active_progs[program_id] = path.string();
            program_id++;
        } else
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } while (std::any_of(executers.begin(),executers.end(), [&](std::shared_ptr<ProgramExecuter>& exec) {
                         return exec->isActive();
                    } ));

    for (auto& elem : executers) {
        elem->Stop();
    }

    writeJSONToFile(report, program_report_file.string());

    return 0;
}

void SignalHandler(int sig) {
    switch (sig)
    {
    case SIGTERM:
        std::cout << "Received SIGTERM signal" << std::endl;
        break;
    case SIGINT:
        std::cout << "Received SIGINT signal" << std::endl;
        break;
    case SIGQUIT:
        std::cout << "Received SIGQUIT signal" << std::endl;
        break;
    }
    sig_interrupt();
}

void sig_interrupt() {
    stop = true;
}
