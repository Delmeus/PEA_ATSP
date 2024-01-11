import subprocess


def parse_results(output):
    lines = output.split('\n')
    path = lines[0]
    cost = lines[1].split('=')[1]
    time = lines[2].split(' ')[-1]
    result = [path, int(cost), int(time)]
    return result


def run_atsp(parameters):

    command = ["G:/Projekty_Studia/PEA_ATSP/GeneticAlgorithm/cmake-build-release/GeneticAlgorithm.exe", "--param1",
               str(parameters["population size"]), "--param2", str(parameters["time"]), "--param3", str(parameters["mutation factor"]),
               "--param4", str(parameters["crossover factor"]), "--param5", str(parameters["file"]), "--param6",
               str(parameters["crossover method"]), "--param7", str(parameters["mutation method"]),
               "--param8", str(parameters["target"])]

    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
    stdout, stderr = process.communicate()

    result = parse_results(stdout)
    return result

