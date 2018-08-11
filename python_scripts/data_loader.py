import numpy as np
import json
import os

class DataLoader(object):
    CommandID = [
        "SAV",
        "NEG",
        "SWP",

        "ADD",
        "SUB",
        "MUL",
        "ADDC",
        "SUBC",
        "MULC",

        "REG",
        "PUSH",
        "POP",
        "PUSHC",
        "LOAD",
        "UNLD",

        "JRO",
        "JEZ",
        "JNZ",
        "JGZ",
        "JLZ",

        "NOP"
    ]    
    @property
    def num_commands(self):
        return len(DataLoader.CommandID)


    def __init__(self,path_to_config):
        self.config = readJSON(path_to_config)
        self.data = []
        path = self.config["dir_to_safe_programs"]
        report = readJSON(self.config["json_to_save_description"])
        for entry in report:
            sub_report = readJSON(os.path.join(path,entry["report_path"]))
            prog_path = os.path.join(path,entry["program_path"])
            for sub_entry in sub_report:
                if os.path.exists(prog_path) and sub_entry["valid"] and not sub_entry["error"]:
                    self.data.append((prog_path,sub_entry["input"],sub_entry["out"]))

    def getEntryByIndex(self,idx):
        prog_path,prog_input,prog_out = self.data[idx]
        prog = readProgramFromFile(prog_path)
        prog_commands = [inner[0] for inner in prog]
        prog_args = [inner[1] for inner in prog]
        return prog_commands,prog_args,prog_input, prog_out

    def getData(self):
        X_command,X_arg,X_input,Y = [],[]
        for idx in range(len(self.data)):
            prog_commands,prog_args,prog_input, prog_out = self.getEntryByIndex(idx)
            X_command.append(prog_commands)
            X_arg.append(prog_args)
            X_input.append(prog_input)
            Y.append(prog_out)
        return X_command,X_arg,X_input,Y

    def getRandomBatch(self,batch_size):
        X_command,X_arg,X_input,Y = [],[]
        for idx in np.random.randint(0,len(self.data),batch_size):
            prog_commands,prog_args,prog_input, prog_out = self.getEntryByIndex(idx)
            X_command.append(prog_commands)
            X_arg.append(prog_args)
            X_input.append(prog_input)
            Y.append(prog_out)
        return X_command,X_arg,X_input,Y

        
        
def readJSON(path):
    if os.path.exists(path) and path.endswith(".json"):
        with open(path, "r") as f:
            data = json.load(f)
    else:
        raise RuntimeError("Incorrect path to config")
    return data

def readProgramFromFile(path):
    program = []
    with open(path, "r") as f:
        for line in f:
            id_,name,arg = line.split()
            program.append( (int(id_), name,int(arg)) )
    return program