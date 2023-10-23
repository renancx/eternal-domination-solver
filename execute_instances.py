import subprocess
import os

# Diretório onde estão os arquivos de entrada
input_dir = "instances2"

# Diretório onde serão armazenadas as saídas
output_dir = "output2"

# Caminho para o executável compilado
executable_path = "./main"

# Verifica se o diretório de saída existe, se não, cria
os.makedirs(output_dir, exist_ok=True)

# Lista todos os arquivos no diretório de entrada
input_files = os.listdir(input_dir)

# Loop sobre cada arquivo de entrada
for input_file in input_files:
    # Constrói os caminhos completos para os arquivos de entrada e saída
    input_path = os.path.join(input_dir, input_file)
    output_path = os.path.join(output_dir, input_file)

    # Comando para executar o resolvedor
    command = [executable_path, input_path]

    # Abre o arquivo de saída para escrita
    with open(output_path, "w") as output_file:
        # Executa o comando e redireciona a saída para o arquivo
        subprocess.run(command, stdout=output_file, stderr=subprocess.PIPE)

print("Processamento concluído.")