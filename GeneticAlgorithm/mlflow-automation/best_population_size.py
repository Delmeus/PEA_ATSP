from multiprocessing.pool import ThreadPool

import mlflow
from simulation import run_atsp
import concurrent.futures
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics


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


def start_test(parameters, filename):

    experiment_name = f"{filename}_population"
    mlflow.set_experiment(experiment_name)
    if mlflow.get_experiment_by_name(experiment_name) is None:
        mlflow.create_experiment(experiment_name)

    mutation_crossover_combinations = [
        {"mutation_method": "s", "crossover_method": "ox"},
        {"mutation_method": "i", "crossover_method": "ox"},
        {"mutation_method": "s", "crossover_method": "ex"},
        {"mutation_method": "i", "crossover_method": "ex"}
    ]

    num_simulations_per_set = 5
    all_costs = []
    all_times = []
    legend_labels = []
    # run algorithm for all parameters
    for parameter in parameters:
        costs = []
        times = []
        print("Next population size = {}, crossover = {}, mutation = {}".format([parameter["population size"]], [parameter["crossover method"]], [parameter["mutation method"]]))
        for i in range(2):
            cost, time = (run_simulations(parameter, num_simulations_per_set))
            cost.sort()
            time.sort()
            for j in range(num_simulations_per_set):
                costs.append(cost[j])
                times.append(time[j])
        population_size = parameter["population size"]
        for combination in mutation_crossover_combinations:
            mutation_method = combination["mutation_method"]
            crossover_method = combination["crossover_method"]
            label = f"{population_size}_{mutation_method}_{crossover_method}"
            legend_labels.append(label)
        costs.sort()
        times.sort()
        all_costs.append(costs)
        all_times.append(times)

    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[0],
             linestyle='-', color='r', label=f'{parameters[0]["population size"]}_s_ox')
    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[1],
             linestyle='--', color='r', label=f'{parameters[1]["population size"]}_s_ox')
    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[2],
             linestyle='-.', color='r', label=f'{parameters[2]["population size"]}_s_ox')

    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[3],
             linestyle='-', color='b', label=f'{parameters[3]["population size"]}_i_ox')
    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[4],
             linestyle='--', color='b', label=f'{parameters[4]["population size"]}_i_ox')
    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[5],
             linestyle='-.', color='b', label=f'{parameters[5]["population size"]}_i_ox')

    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[6],
             linestyle='-', color='g', label=f'{parameters[6]["population size"]}_s_ex')
    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[7],
             linestyle='--', color='g', label=f'{parameters[7]["population size"]}_s_ex')
    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[8],
             linestyle='-.', color='g', label=f'{parameters[8]["population size"]}_s_ex')

    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[9],
             linestyle='-', color='k', label=f'{parameters[9]["population size"]}_i_ex')
    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[10],
             linestyle='--', color='k', label=f'{parameters[10]["population size"]}_i_ex')
    plt.plot(np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), np.array(all_costs)[11],
             linestyle='-.', color='k', label=f'{parameters[11]["population size"]}_i_ex')

    plt.legend()
    plt.xlabel("Pomiar")
    plt.ylabel("Koszt rozwiązania")
    plt.title("Koszty rozwiązań dla różnych wielkości populacji")

    plt.tight_layout(rect=(0, 0, 0.85, 1))

    plt.legend(bbox_to_anchor=(0.95, 1), loc='upper left')
    plt.grid()

    plt.savefig("ftv47_diff_pop.png", format='png', bbox_inches='tight')
    plt.show()

    medium_costs = []

    for cost_list in all_costs:
        medium_costs.append(statistics.median(cost_list))

    table = pd.DataFrame({'Cost': medium_costs},
                         index=["ScrambleOX 500", "ScrambleOX 1500", "ScrambleOX 2500",
                                "InversionOX 500", "InversionOX 1500", "InversionOX 2500",
                                "ScrambleEX 500", "ScrambleEX 1500", "ScrambleEX 2500",
                                "InversionEX 500", "InversionEX 1500", "InversionEX 2500",
                                ])

    print(table)
    table.to_csv(f'{filename}_best_pop.csv', index=True, sep=';', decimal=',')



