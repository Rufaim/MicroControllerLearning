#ifndef COMMAD_LIST_H
#define COMMAD_LIST_H

#include <stdexcept>

class command_error : public std::runtime_error {
public:
    explicit command_error(const std::string& arg) : runtime_error(arg) {}
};

enum CommandID {
    SAV,
    NEG,
    SWP,

    ADD,
    SUB,
    MUL,
    ADDC,
    SUBC,
    MULC,

    REG,
    PUSH,
    POP,
    PUSHC,
    LOAD,
    UNLD,

    JRO,
    JEZ,
    JNZ,
    JGZ,
    JLZ,

    NOP
};

enum ArgumentType {
    NONE,
    INPUT_REG,
    ACTIVE_REG,
    OUTPUT_REG,
    CONSTANT,
    POINTER,
};

struct Command {
    CommandID id;
    ArgumentType argtype;
    std::string name;
};


Command getCommandByID(CommandID);


#endif // COMMAD_LIST_H
