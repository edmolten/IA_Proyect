import subprocess
from os import walk
INSTANCES_PATH = "/home/edmolten/IA_Proyect/IA/instances"
PROGRAM_PATH = "/home/edmolten/IA_Proyect/main"
ITERATIONS_CATEGORY_NAME = [
    (3000, 1000, "Chart1"),
    (1000, 1000, "Chart4"),
    (100, 50, "Chart16"),
    (100, 50, "Chart17"),
    (100, 50, "Chart18")
]
for base_iterations, step_iteration, category_name in ITERATIONS_CATEGORY_NAME:
    for it in range(base_iterations, step_iteration*5 + 1, step_iteration):
        for root, dirs, files in walk(INSTANCES_PATH + "/" + category_name):
            for file in files:
                path = root + "/" + file
                command = ("time", PROGRAM_PATH, path, str(it))
                with subprocess.Popen(command, stdout=subprocess.PIPE) as proc:
                    out_file = open(category_name + "." + file.split(".")[0] + "." + str(it), "w")
                    out = proc.stdout.read().decode("utf-8")
                    print(out)
                    out_file.write(out)
                    out_file.close()
