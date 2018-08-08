import json
import os
from copy import copy

PATH = "./report.json"
PATH_1 = "./report_1.json"

with open(PATH,"r") as file:
	data = json.load(file)

data_new = []

for entry in data:
	_,p_path = os.path.split(entry["program_path"])
	_,r_path = os.path.split(entry["report_path"])
	new_entry = copy(entry)
	new_entry["program_path"] = p_path
	new_entry["report_path"] = r_path
	data_new.append(new_entry)

with open(PATH_1,"w") as file:
	json.dump(data_new,file,indent=1)