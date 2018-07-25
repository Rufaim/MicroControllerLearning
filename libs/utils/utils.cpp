#include "utils.h"

#include <sstream>
#include <fstream>
#include <string>

template<class T>
void print_vector(std::vector<T> vect) {
    std::cout << "Size: " << vect.size() << std::endl;
    for (T elem : vect)
        std::cout << elem << " ";
    std::cout << std::endl;
}


void writeProgramToFile(std::vector<std::pair<CommandID, int> > &program, std::string file) {
    std::ofstream out(file, std::ios_base::out | std::ios_base::trunc);
    for (std::pair<CommandID, int> elem : program) {
            Command comm = getCommandByID(elem.first);
            out << comm.id << "\t" << comm.name << "\t" << elem.second << std::endl;
    }
    out.close();
}

void readProgramFromFile(std::vector<std::pair<CommandID, int> > &program, std::string file) {
    program.clear();
    std::ifstream in(file, std::ios_base::in);
    int id, arg;
    std::string name;
    while(in >> id >> name >> arg)
        program.push_back(std::make_pair(CommandID(id),arg));
    in.close();
}

void writeJSONToFile(json &json_file, std::string filename) {
    std::ofstream out(filename, std::ios_base::out | std::ios_base::trunc);
    out << std::setw(4) << json_file;
    out.close();
}

void readJSONFromFile(json &json_file, std::string filename) {
    std::ifstream in(filename, std::ios_base::in);
    in >> json_file;
    in.close();
}
