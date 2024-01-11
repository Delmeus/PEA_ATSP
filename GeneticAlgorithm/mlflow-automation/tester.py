from multiprocessing.pool import ThreadPool

import mlflow
from simulation import run_atsp
import concurrent.futures
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics

mutation_crossover_combinations = [
    {"mutation_method": "s", "crossover_method": "ox"},
    {"mutation_method": "i", "crossover_method": "ox"},
    {"mutation_method": "s", "crossover_method": "pmx"},
    {"mutation_method": "i", "crossover_method": "pmx"}
]

legend_labels = []


def run_experiment(parameters, iteration):
    with mlflow.start_run(
            run_name=f"{parameters['population size']}x{parameters['crossover factor']}x{parameters['mutation factor']}_{iteration + 1}"):
        result = run_atsp(parameters)
        path, cost, time = result
        mlflow.log_params(parameters)
        mlflow.log_param("path", path)
        mlflow.log_metric("cost", cost)
        mlflow.log_metric("time", time)

        print("cost = {}, path = {}".format(cost, path))
        return cost, time


def run_simulations(parameters, num_simulations_per_set):
    costs = []
    times = []
    with concurrent.futures.ThreadPoolExecutor() as executor:
        futures = [executor.submit(run_atsp, parameters) for _ in range(num_simulations_per_set)]
    for i, future in enumerate(futures):
        with mlflow.start_run(
                run_name=f"{parameters['population size']}x{parameters['mutation method']}x{parameters['crossover method']}"):
            mlflow.log_params(parameters)
            result = future.result()
            path, cost, time = result
            mlflow.log_param("path", path)
            mlflow.log_metric("cost", cost)
            mlflow.log_metric("time", time)
            print("cost = {}, path = {}".format(cost, path))
            costs.append(cost)
            times.append(time)

    return costs, times


def run_and_label(label, parameter, num_simulations_per_set):
    costs = []
    times = []
    for i in range(2):
        cost, time = (run_simulations(parameter, num_simulations_per_set))
        cost.sort()
        time.sort()
        for j in range(num_simulations_per_set):
            costs.append(cost[j])
            times.append(time[j])
    desired_label = parameter[label]
    for combination in mutation_crossover_combinations:
        mutation_method = combination["mutation_method"]
        crossover_method = combination["crossover_method"]
        label = f"{desired_label}_{mutation_method}_{crossover_method}"
        legend_labels.append(label)
    return costs, times


def run_population_tests(parameters, num_simulations_per_set):
    all_costs = []
    all_times = []

    # run algorithm for all parameters
    for parameter in parameters:
        print("Next population size = {}, crossover = {}, mutation = {}".format([parameter["population size"]], [parameter["crossover method"]], [parameter["mutation method"]]))
        costs, times = run_and_label("population size", parameter, num_simulations_per_set)
        costs.sort()
        times.sort()
        all_costs.append(costs)
        all_times.append(times)
    return all_costs, all_times


def run_crossovers_tests(parameters, num_simulations_per_set):
    all_costs = []
    all_times = []
    for parameter in parameters:
        print("Currently testing {} crossover with {} mutation for crossover factor {}".format(parameter["crossover method"], parameter["mutation method"], parameter["crossover factor"]))
        costs, times = run_and_label("crossover factor", parameter, num_simulations_per_set)
        costs.sort()
        times.sort()
        all_costs.append(costs)
        all_times.append(times)
    return all_costs, all_times


def run_mutations_tests(parameters, num_simulations_per_set):
    all_costs = []
    all_times = []
    for parameter in parameters:
        print("Currently testing {} crossover with {} mutation for mutation factor {}".format(parameter["crossover method"], parameter["mutation method"], parameter["mutation factor"]))
        costs, times = run_and_label("mutation factor", parameter, num_simulations_per_set)
        costs.sort()
        times.sort()
        all_costs.append(costs)
        all_times.append(times)
    return all_costs, all_times


def start_test(parameters, filename, test):

    if test == 1:
        experiment_name = f"{filename}_population_sprawko"
    elif test == 2:
        experiment_name = f"{filename}_crossover_sprawko"
    else:
        experiment_name = f"{filename}_mutation_sprawko"
        
    mlflow.set_experiment(experiment_name)
    if mlflow.get_experiment_by_name(experiment_name) is None:
        mlflow.create_experiment(experiment_name)

    num_simulations_per_set = 5

    if test == 1:
        all_costs, all_times = run_population_tests(parameters, num_simulations_per_set)

        numbers = []
        for j in range(len(all_costs)):
            for i in range(1, len(all_costs[0]) + 1):
                numbers.append(i)

        plot(numbers, all_costs, parameters, "population size", "Pomiar", "Koszt rozwiązania",
             "Koszty rozwiązań dla różnych wielkości populacji", filename, "diff_pop")

        medium_costs = []

        for cost_list in all_costs:
            medium_costs.append(statistics.median(cost_list))

        table = pd.DataFrame({'Cost': medium_costs},
                             index=["ScrambleOX 200", "ScrambleOX 500", "ScrambleOX 750",
                                    "InversionOX 200", "InversionOX 500", "InversionOX 750",
                                    "ScramblePMX 200", "ScramblePMX 500", "ScramblePMX 750",
                                    "InversionPMX 200", "InversionPMX 500", "InversionPMX 750",
                                    ])

        print(table)
        table.to_csv(f'{filename}_best_pop.csv', index=True, sep=';', decimal=',')

    elif test == 2:
        all_costs, all_times = run_crossovers_tests(parameters, num_simulations_per_set)
        plot(all_times, all_costs, parameters, "crossover factor", "Czas znalezienia rozwiązania [ms]", "Koszt rozwiązania",
             "Koszty rozwiązań dla różnych wartości współczynnika krzyżowania", filename, "diff_cf")
        f = open(f'{filename}_crossovers.csv', "w")

        names = ["ScrambleOX 0.5", "ScrambleOX 0.7", "ScrambleOX 0.9",
                 "InversionOX 0.5", "InversionOX 0.7", "InversionOX 0.9",
                 "ScramblePMX 0.5", "ScramblePMX 0.7", "ScramblePMX 0.9",
                 "InversionPMX 0.5", "InversionPMX 0.7", "InversionPMX 0.9",
                 ]

        for i in range(len(all_costs)):
            f.write("\n" + names[i] + "\n" + "Koszt" + ";" + "Czas [ms]" + "\n")
            for j in range(len(all_costs[i])):
                f.write(str(all_costs[i][j]) + ";" + str(all_times[i][j]) + "\n")
        f.close()
    else:
        all_costs, all_times = run_mutations_tests(parameters, num_simulations_per_set)
        plot(all_times, all_costs, parameters, "mutation factor", "Czas znalezienia rozwiązania [ms]", "Koszt rozwiązania",
             "Koszty rozwiązań dla różnych wartości współczynnika mutacji", filename, "diff_cf")
        f = open(f'{filename}_mutations.csv', "w")

        names = ["ScrambleOX 0.01", "ScrambleOX 0.05", "ScrambleOX 0.10",
                 "InversionOX 0.01", "InversionOX 0.05", "InversionOX 0.10",
                 "ScramblePMX 0.01", "ScramblePMX 0.05", "ScramblePMX 0.10",
                 "InversionPMX 0.01", "InversionPMX 0.05", "InversionPMX 0.10",
                 ]

        for i in range(len(all_costs)):
            f.write("\n" + names[i] + "\n" + "Koszt" + ";" + "Czas [ms]" + "\n")
            for j in range(len(all_costs[i])):
                f.write(str(all_costs[i][j]) + ";" + str(all_times[i][j]) + "\n")
        f.close()


def plot(numbers, all_costs, parameters, parameter, x_label, y_label, title, filename, file_title):
    plt.plot(np.array(numbers[0]), np.array(all_costs)[0],
             linestyle='-', color='r', label=f'{parameters[0][parameter]}_s_ox')
    plt.plot(np.array(numbers[1]), np.array(all_costs)[1],
             linestyle='--', color='r', label=f'{parameters[1][parameter]}_s_ox')
    plt.plot(np.array(numbers[2]), np.array(all_costs)[2],
             linestyle='-.', color='r', label=f'{parameters[2][parameter]}_s_ox')

    plt.plot(np.array(numbers[3]), np.array(all_costs)[3],
             linestyle='-', color='b', label=f'{parameters[3][parameter]}_i_ox')
    plt.plot(np.array(numbers[4]), np.array(all_costs)[4],
             linestyle='--', color='b', label=f'{parameters[4][parameter]}_i_ox')
    plt.plot(np.array(numbers[5]), np.array(all_costs)[5],
             linestyle='-.', color='b', label=f'{parameters[5][parameter]}_i_ox')

    plt.plot(np.array(numbers[6]), np.array(all_costs)[6],
             linestyle='-', color='g', label=f'{parameters[6][parameter]}_s_pmx')
    plt.plot(np.array(numbers[7]), np.array(all_costs)[7],
             linestyle='--', color='g', label=f'{parameters[7][parameter]}_s_pmx')
    plt.plot(np.array(numbers[8]), np.array(all_costs)[8],
             linestyle='-.', color='g', label=f'{parameters[8][parameter]}_s_pmx')

    plt.plot(np.array(numbers[9]), np.array(all_costs)[9],
             linestyle='-', color='k', label=f'{parameters[9][parameter]}_i_pmx')
    plt.plot(np.array(numbers[10]), np.array(all_costs)[10],
             linestyle='--', color='k', label=f'{parameters[10][parameter]}_i_pmx')
    plt.plot(np.array(numbers[11]), np.array(all_costs)[11],
             linestyle='-.', color='k', label=f'{parameters[11][parameter]}_i_pmx')

    plt.legend()
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(title)

    plt.tight_layout(rect=(0, 0, 0.85, 1))

    plt.legend(bbox_to_anchor=(0.95, 1), loc='upper left')
    plt.grid()

    plt.savefig(f"{filename}_{file_title}.png", format='png', bbox_inches='tight')
    plt.show()
