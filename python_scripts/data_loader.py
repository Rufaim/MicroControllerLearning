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
    _command_to_id_dict = {comm:i for i,comm in enumerate(CommandID)}
    @property
    def num_commands(self):
        return len(DataLoader.CommandID)
    @property
    def num_inputs(self):
        return self.config["num_inputs_regs"]
    @property
    def num_outputs(self): 
        return self.config["num_outputs_regs"]
    @property
    def max_program_length(self): 
        return self.config["generator"]["max_program_len"] + 2*self.num_outputs

    def to_num(command):
        return DataLoader._command_to_id_dict[command]
    def from_num(num):
        return DataLoader.CommandID[num]


    def __init__(self,path_to_config):
        self.config = readJSON(path_to_config)
        self.data = []
        path = self.config["dispatcher"]["dir_to_safe_programs"]
        report = readJSON(self.config["dispatcher"]["json_to_save_description"])
        for entry in report:
            prog_path = os.path.join(path,entry["program_path"])
            if os.path.isfile(prog_path):
                sub_report = readJSON(os.path.join(path,entry["report_path"]))      
                for sub_entry in sub_report['enties']:
                    self.data.append((prog_path,sub_entry["in"],sub_entry["out"]))

    def getEntryByIndex(self,idx):
        prog_path,prog_input,prog_out = self.data[idx]
        prog = readProgramFromFile(prog_path)
        prog_commands = [inner[0] for inner in prog]
        prog_args = [inner[2] for inner in prog]
        return prog_commands,prog_args,prog_input, prog_out

    def getData(self):
        X_command,X_arg,X_input,Y = [],[],[],[]
        for idx in range(len(self.data)):
            prog_commands,prog_args,prog_input, prog_out = self.getEntryByIndex(idx)
            X_command.append(prog_commands)
            X_arg.append(prog_args)
            X_input.append(prog_input)
            Y.append(prog_out)
        return postProcess(X_command,X_arg,X_input,Y,self.max_program_length)

    def getRandomBatch(self,batch_size):
        X_command,X_arg,X_input,Y = [],[],[],[]
        for idx in np.random.randint(0,len(self.data),batch_size):
            prog_commands,prog_args,prog_input, prog_out = self.getEntryByIndex(idx)
            X_command.append(prog_commands)
            X_arg.append(prog_args)
            X_input.append(prog_input)
            Y.append(prog_out)
        return postProcess(X_command,X_arg,X_input,Y,self.max_program_length)

def postProcess(X_command,X_arg,X_input,Y,max_len):
    lens = [len(comm) for comm in X_command]
    X_command_new, X_arg_new = [],[]
    for idx, (x_c, x_a) in enumerate(zip(X_command,X_arg)):
        X_command_new.append(x_c + [0 for i in range(max_len-lens[idx])])
        X_arg_new.append(x_a + [0 for i in range(max_len-lens[idx])])
    X_command = np.array(X_command_new)
    X_arg = np.array(X_arg_new)
    X_input = np.array(X_input)
    Y = np.array(Y)
    lens = np.array(lens)

    return X_command,X_arg,X_input,Y, lens
        
def readJSON(path):
    if os.path.isfile(path) and path.endswith(".json"):
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