from tester import start_test

if __name__ == "__main__":
    filename = "ftv47"
    path = "G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\"
    time = 1
    target = 1776
    size = 500
    # define parameters for algorithm
    parameters_small = [
        # order crossover and scramble mutate
        {"population size": 200, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        {"population size": 500, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        {"population size": 750, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        # order crossover and inversion mutate
        {"population size": 200, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        {"population size": 500, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        {"population size": 750, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        # pmx and scramble mutate
        {"population size": 200, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        {"population size": 500, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        {"population size": 750, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        # pmx and inversion mutate
        {"population size": 200, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target},
        {"population size": 500, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target},
        {"population size": 750, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target}
    ]

    parameters_crossover = [
        # order crossover and scramble mutate
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.5,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.7,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.9,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        # order crossover and inversion mutate
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.5,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.7,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.9,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        # pmx and scramble mutate
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.5,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.7,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.9,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        # pmx and inversion mutate
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.5,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.7,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.9,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target}
    ]

    parameters_mutations = [
        # order crossover and scramble mutate
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.05, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.1, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        # order crossover and inversion mutate
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.05, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.1, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        # pmx and scramble mutate
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.05, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.1, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        # pmx and inversion mutate
        {"population size": size, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.05, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target},
        {"population size": size, "time": time, "mutation factor": 0.1, "crossover factor": 0.8,
         "file": f"{path}input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target}
    ]

    start_test(parameters_small, filename, 1)
    # start_test(parameters_crossover, filename, 2)
    # start_test(parameters_mutations, filename, 3)
