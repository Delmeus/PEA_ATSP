from best_population_size import start_test

if __name__ == "__main__":
    filename = "ftv47"
    # define parameters for algorithm
    parameters_small = [
        # order crossover and scramble mutate
        {"population size": 500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": 1776},
        {"population size": 1500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": 1776},
        {"population size": 2500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 0, "target": 1776},
        # order crossover and inversion mutate
        {"population size": 500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": 1776},
        {"population size": 1500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": 1776},
        {"population size": 2500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 0, "mutation method": 1, "target": 1776},
        # pmx and scramble mutate
        {"population size": 500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": 1776},
        {"population size": 1500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": 1776},
        {"population size": 2500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 0, "target": 1776},
        # pmx and inversion mutate
        {"population size": 500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": 1776},
        {"population size": 1500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": 1776},
        {"population size": 2500, "time": 120, "mutation factor": 0.01, "crossover factor": 0.8,
         "file": f"G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\{filename}.atsp",
         "crossover method": 1, "mutation method": 1, "target": 1776}
    ]

    start_test(parameters_small, filename)
