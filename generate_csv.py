def read_dat_file(file_name):
    """
    Lê o conteúdo de um arquivo de saída e retorna as informações desejadas.

    Args:
        file_name (str): O nome do arquivo de saída.

    Returns:
        Uma lista com o nome da instância, o maior valor de k (se encontrado) e o tempo de execução.
    """
    with open(file_name, "r") as f:
        lines = f.readlines()

    instance_name = None
    max_k = None
    running_time = None

    for line in lines:
        if line.startswith("Instance:"):
            instance_name = line.split(":")[1].strip()
        elif line.startswith("k:"):
            k = int(line.split(":")[1].strip())
            if max_k is None or k > max_k:
                max_k = k
        elif line.startswith("Running time:"):
            running_time = int(line.split(":")[1].strip().split()[0])  # Extrai o tempo em milissegundos
        elif line.startswith("Time limit exceeded:"):
            if max_k is None:
                max_k = "N/A"  # Não foi encontrado um valor de k dentro do tempo limite
            if running_time is None:
                running_time = int(line.split(":")[1].strip().split()[0])  # Extrai o tempo em milissegundos

    return [instance_name, max_k, running_time]

import csv
import os

def main():
    """
    Lê todos os arquivos .dat e escreve os resultados em um arquivo CSV.
    """

    # Lista todos os arquivos .dat em ordem crescente
    input_files = sorted(os.listdir("results"), key=lambda x: x.split(".dat")[0])

    # Cria o arquivo CSV.
    with open("results.csv", "w") as f:
        writer = csv.writer(f)
        writer.writerow(["Instance", "Guard Set", "Time (ms)"])

    # Lê todos os arquivos .dat
    for file_name in input_files:
        if file_name.endswith(".dat"):
            # Lê o conteúdo do arquivo.
            data = read_dat_file(os.path.join("results", file_name))

            # Escreve no arquivo CSV.
            with open("results.csv", "a") as f:
                writer = csv.writer(f)
                writer.writerow(data)

if __name__ == "__main__":
    main()