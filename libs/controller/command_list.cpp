#include "command_list.h"

Command getCommandByID(CommandID id) {
    switch (id) {
    case CommandID::NOP:
        return { CommandID::NOP, ArgumentType::NONE, "NOP" };
    case CommandID::SAV:
        return { CommandID::SAV, ArgumentType::NONE, "SAV" };
    case CommandID::NEG:
        return { CommandID::NEG, ArgumentType::NONE, "NEG" };
    case CommandID::SWP:
        return { CommandID::SWP, ArgumentType::NONE, "SWP" };

    case CommandID::ADD:
        return { CommandID::ADD, ArgumentType::ACTIVE_REG, "ADD" };
    case CommandID::SUB:
        return { CommandID::SUB, ArgumentType::ACTIVE_REG, "SUB" };
    case CommandID::MUL:
        return { CommandID::MUL, ArgumentType::ACTIVE_REG, "MUL" };
    case CommandID::ADDC:
        return { CommandID::ADDC, ArgumentType::CONSTANT, "ADDC" };
    case CommandID::SUBC:
        return { CommandID::SUBC, ArgumentType::CONSTANT, "SUBC" };
    case CommandID::MULC:
        return { CommandID::MULC, ArgumentType::CONSTANT, "MULC" };

    case CommandID::REG:
        return { CommandID::REG, ArgumentType::ACTIVE_REG, "REG" };
    case CommandID::PUSH:
        return { CommandID::PUSH, ArgumentType::ACTIVE_REG, "PUSH" };
    case CommandID::POP:
        return { CommandID::POP, ArgumentType::ACTIVE_REG, "POP" };
    case CommandID::PUSHC:
        return { CommandID::PUSHC, ArgumentType::CONSTANT, "PUSHC" };
    case CommandID::LOAD:
        return { CommandID::LOAD, ArgumentType::INPUT_REG, "LOAD" };
    case CommandID::UNLD:
        return { CommandID::UNLD, ArgumentType::OUTPUT_REG, "UNLD" };

    case CommandID::JRO:
        return { CommandID::JRO, ArgumentType::POINTER, "JRO" };
    case CommandID::JEZ:
        return { CommandID::JEZ, ArgumentType::POINTER, "JEZ" };
    case CommandID::JNZ:
        return { CommandID::JNZ, ArgumentType::POINTER, "JNZ" };
    case CommandID::JGZ:
        return { CommandID::JGZ, ArgumentType::POINTER, "JGZ" };
    case CommandID::JLZ:
        return { CommandID::JLZ, ArgumentType::POINTER, "JLZ" };

    default:
        throw command_error("wrong command ID");
    }
}

