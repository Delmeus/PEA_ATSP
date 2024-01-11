from best_population_size import start_test

if __name__ == "__main__":
    filename = "ftv47"
    time = 120
    target = 1776
    # define parameters for algorithm
    parameters_small = [
        # order crossover and scramble mutate
        {"population size": 200, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        {"population size": 500, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        {"population size": 750, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": target},
        # order crossover and inversion mutate
        {"population size": 200, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        {"population size": 500, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        {"population size": 750, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": target},
        # pmx and scramble mutate
        {"population size": 200, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        {"population size": 500, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        {"population size": 750, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": target},
        # pmx and inversion mutate
        {"population size": 200, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target},
        {"population size": 500, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target},
        {"population size": 750, "time": time, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"C:\\Users\\antek\\Desktop\\studia\\5.sem\\Pea sprawka i wyniki\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": target}
    ]

    start_test(parameters_small, filename)
