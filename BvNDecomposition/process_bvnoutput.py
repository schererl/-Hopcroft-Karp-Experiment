import pandas as pd

# Define the path to the log file
file_path = './log-execution-60s.out'

# Read the content of the log file
with open(file_path, 'r') as file:
    log_lines = file.readlines()

# Extract relevant information from the log lines
data = []
file_name = None
vertices = None
edges = None
iterations = None
elapsed_time = None
alpha_sum = None

for line in log_lines:
    if line.startswith("openning next file:"):
        if file_name and vertices is not None and edges is not None and iterations is not None and elapsed_time is not None and alpha_sum is not None:
            data.append([file_name, elapsed_time, vertices, edges, iterations, alpha_sum])
        file_name = None
        vertices = None
        edges = None
        iterations = None
        elapsed_time = None
        alpha_sum = None
    if line.strip().endswith(".dsm.mtx.gz"):
        file_name = line.strip()
    if line.startswith("Header read:"):
        parts = line.split()
        vertices = int(parts[2])
        edges = int(parts[4])
    if line.startswith("Iterations:"):
        iterations = int(line.split()[1])
    if line.startswith("Elapsed Time:"):
        elapsed_time = float(line.split()[2])
    if line.startswith("Alpha Sum:"):
        alpha_sum = float(line.split()[2])

# Append the last entry if it exists
if file_name and vertices is not None and edges is not None and iterations is not None and elapsed_time is not None and alpha_sum is not None:
    data.append([file_name, elapsed_time, vertices, edges, iterations, alpha_sum])

# Create a DataFrame with the extracted data
df = pd.DataFrame(data, columns=["File Name", "Elapsed Time (s)", "Number of Vertices", "Number of Edges", "Number of Iterations", "Alpha Sum"])

print(df)