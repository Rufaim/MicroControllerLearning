#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

#include "json/json.hpp"
#include "controller/controller.h"

using namespace nlohmann;

template<class T>
void print_vector(std::vector<T> vect);

void writeJSONToFile(json& json_file, std::string filename);
void readJSONFromFile(json& json_file, std::string filename);

void writeProgramToFile(std::vector<std::pair<CommandID,int>> &program, std::string file);
void readProgramFromFile(std::vector<std::pair<CommandID,int>> &program, std::string file);

#endif // UTILS_H
