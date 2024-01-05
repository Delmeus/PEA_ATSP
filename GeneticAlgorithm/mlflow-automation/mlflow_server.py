import mlflow
from simulation import run_simulation
from concurrent.futures import ThreadPoolExecutor


def run_test(test_num, parameters):
    with mlflow.start_run(run_name=f"test_{test_num}"):
        result = run_simulation(parameters)
        mlflow.log_params(parameters)
        mlflow.log_param("path", result[0])
        mlflow.log_metric("cost", float(result[1]))
        mlflow.log_metric("time", float(result[2]))


if __name__ == "__main__":
    # Set MLflow experiment name
    experiment_name = "test"

    # Check if the experiment already exists, if not, create it
    if mlflow.get_experiment_by_name(experiment_name) is None:
        mlflow.create_experiment(experiment_name)

    # Run simulation and log with MLflow

        # Define parameters for the simulation
    parameters_global = {
        "population size": 100,
        "time": 10,
        "mutation factor": 0.01,
        "crossover factor": 0.8,
        "file": "G:\\Projekty_Studia\\PEA_ATSP\\GeneticAlgorithm\\input\\ftv47.atsp",
        "crossover method": 0,
        "mutation method": 0
    }

    num_tests = 3
    with ThreadPoolExecutor() as executor:
        executor.map(run_test, range(1, num_tests + 1), [parameters_global] * num_tests)

